# Sample code for Hitachi Hokudai Labo and Hokkaido University Contest 2019-1 B

* This is an explanation of the sample code used in the Hitachi Hokudai Labo and Hokkaido University Contest 2019-1 B. Based on the distributed code the participants can create input samples and evaluate the score of their output on their own local machine.
* Please note that you are using the provided sample code on your own responsibility. In particular, neither Hokkaido University nor Hitachi Ltd. can be held responsible or liable, in case the execution of the provided sample code causes any damage to your computer.
* The provided program for score evaluation is identical with the program which will be used to evaluate your score during the contest. However, since the random seed used to generate the input cases during the contest may vary from your input cases, the final score of your program might vary.

## Sample code overview

The sample code is written in C++ and can be found in the subfolder `scripts`.

* `judge.cpp`
    * This program is the game engine. It is used
* `interactive.py`
    * This program is used to run the judge program and the contestant code in an interactive manner.
* `sample_code/sample.cpp`, `sample_code/sample.py`
    * These codes are simple submissions examples to kick start contest beginners. They take care of handling the input / output communication for you. However, to make them score, you have to put your own ideas into practice.

## Compilation

In order to create an executable file called `judge`, compile `judge.cpp` as follows.

```bash
g++ -std=gnu++11 -o judge judge.cpp
```

* Note: In case the C++11 flag is ignored, there is some chance that your compiler terminates with an error.

In addition, when using this toolkit in the contestants local environment, the contestant needs to compile their code into an executable file. Henceforth, the contestant executable will be referred to as `contestant`.

## Test case generation and interactive processing

In order to generate input samples and execute the contestant code, please
execute `interactive.py` as follows:

```bash
Usage: ./interactive.py --judge './judge <case-type> <seed>' --contestant ./contestant
```

Here, `interactive.py` uses the following arguments:
- `<case-type>`: Specifies the type of graph
    - If `1`: Creates a graph with 200 to 400 vertices.
    - If `2`: Creates a graph with 400 vertices.
- `<seed>`: an integer which specifies a random seed for the graph generation
- `./contestant`: the execution of the executable contestant code called `contestant`

For example, running the contestant file `contestant` on a graph of type `1`, using the seed 2048 is done as follows:

```bash
./interactive.py --judge './judge 1 2048' --contestant ./contestant
```

When `interactive.py` is executed the messages of the contestant code and the judge code will be send to the standard output. The last line of the standard output is your score. In addition, two files will be generated:
- `contestant.out` : Log file of the commands send by contestant to the standard output
- `judge.out`: Log file of the messages send by host code to the standard output

Note: Based on `contestant.out` and `judge.out` the contestant can visualize their result using the contest visualizer distributed with this contest. For details refer to the README.md in the distributed visualizer.

## Acknowledgement

The code in `interactive.py` originates from the following repository `https://github.com/sharmaeklavya2/croupier` and is licensed under the [MIT Licence](https://opensource.org/licenses/MIT).
