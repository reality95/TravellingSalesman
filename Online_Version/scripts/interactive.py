#!/usr/bin/env python3

import os, sys, subprocess
import shlex, threading, argparse

MAX_BYTE = 1000

def parts_to_print_gen(print_buffer, parts):
    yield print_buffer
    for part in parts[1: -1]:
        yield part

def transfer_and_print(f1, f2, name, filename=''):
    with open(filename, "w") as f:
        print_buffer = bytearray()
        while(True):
            ch = os.read(f1, MAX_BYTE)
            if ch == b'':
                break

            parts = ch.split(b'\n')
            print_buffer += parts[0]
            if len(parts) > 1:
                for part in parts_to_print_gen(print_buffer, parts):
                    try:
                        s = part.decode()
                    except UnicodeDecodeError:
                        s = str(bytes(part))
                    if name:
                        print("{}: {}".format(name, s))
                        print(s, file=f, flush=True)
                    else:
                        print(s, file=stdout)
                        print(s, file=f, flush=True)
                print_buffer = bytearray(parts[-1])
            os.write(f2, ch)


def logged_pipe(name='', filename=''):
    f1, write_end = os.pipe()
    read_end, f2 = os.pipe()
    args = (f1, f2, name, filename)
    thread = threading.Thread(target=transfer_and_print, args=args)
    thread.daemon = True
    thread.start()
    return (read_end, write_end)

def interact(pstr1, pstr2, name1, name2, filename1, filename2):
    pargs1 = shlex.split(pstr1)
    pargs2 = shlex.split(pstr2)
    a_to_b = logged_pipe(name1, filename1)
    b_to_a = logged_pipe(name2, filename2)
    pa = subprocess.Popen(pargs1, stdin=b_to_a[0], stdout=a_to_b[1])
    pb = subprocess.Popen(pargs2, stdin=a_to_b[0], stdout=b_to_a[1])
    pa.wait()
    pb.wait()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--judge', help='Command line of judge execution file')
    parser.add_argument('--contestant', help='Command line of contestant execution file')
    parser.add_argument('--filename1', default='judge.out', help='Output file name of judge process')
    parser.add_argument('--filename2', default='contestant.out', help='Output file name of contestant process')
    args = parser.parse_args()

    interact(args.judge, args.contestant, 'Judge', 'Contestant', args.filename1, args.filename2)


if __name__ == '__main__':
    main()
