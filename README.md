# c-bitwise-operators
A simple program that tries to simutale a encoder/decoder for addition, subtraction, multiplication and division

# Table of Contents
1. [Getting Started](#start-description)
2. [Building the Project](#build-description)
3. [Using the Project](#use-description)

<a name="start-description"></a>
## Getting Started

The project is a **miniprocessor** that tries to execute some instructions.
An instruction is **encoded** on an **unsigned int** and has the following pattern:

* First 3 bits are for **number of operations** <= 8
* The next **2 * number_of_operations** bits are for the operations itself "+, -, *, /"
* The next 4 bits are for **dimension** of an operation member (If you want to add numbers of 11 bits that the dimension has to be **11**) <= 16 (working with short ints)

The project comes with **3** source files.

* **encoder.c** - The file that will take your instruction and will encode it for the program processor. The writing instructions will  be discussed in **Using the Project** .

* **opernum.c** - After you encoded your instruction you must rum the opernum.c with the encoded instruction as **input** data to check how many operator members you should provide. A operator member is a **unsigned short int** number that we will extract the desired numbers of desired dimension.

* **decoder.c** - After you checked for number of operator members you shall to execute your instruction. You should provide for the program the encoded instruction and the **operator members** (short ints). The prgram has 4 modes:
    * (0) Show the decoded instruction in the specified file
    * (1) Execute the instruction considering that the dimension is a power of two from
    [1, 16] and the precendence of the operators does NOT matter.
    * (2) Execute the instruction considering that the dimension is any number form [1, 16] 
    and the precedence of the operators does NOT matter.
    * (3) Execute the instruction considering that the dimension is any number form [1, 16] 
    and the precedence of the operators DOES matter.

<a name="build-description"></a>
## Building the Project

In order to build the project make sure that you have the **gcc compiler** installed, if not you will have to build the files by yourself.

That make sure that you cloned the project and changed the directory into it:

```BASH
    cd build
    make
```

After above commands you will get 3 executables:

* encoder
* opernum
* decoder

To clean every junk files after working with files run:

```BASH
    make clean
```

<a name="use-description"></a>
## Using the Project

In order to work with project make sure that you build the project.

Now let's see how to write an instruction:

```TXT
    "In input.txt (or any other file)"

    5 11 * / + - *
```

First we write the number of operations, then the dimension of one member and after that we write the desired operations.

To encode the file run:

```BASH
    ./encoder input.txt encoded.txt
```

in the file you will get the following number:

```BASH
    cat encoded.txt
    2520055808
```

The number is your instruction encoded.

Now you should run the **opernum** to fin how many numbers you should insert:

```BASH
    ./opernum < encoded.txt
    5
```

Now you now that you should write down 5 operator members.

Open the encoded.txt file and write after the encoded instruction those 5 members:
!!! The members have to be **short ints**

```TXT
    "In encoded.txt"

    2520055808
    5177
    11625
    64043
    45425
    57098
```

Now after you write the members you should run the decode program with specified mod selector:

```BASH
    ./decoder encoded.txt output.txt 3
    cat output.txt
    -2210220
```

As mentioned you have 4 modes indexed from 0.

Explanation:

From the **5** members the program extracted 6 new members by dimension **11** as follows:

```
    159 1867 723 1954 1496 1479
```

Adding the operations the execution shows like:

```
    159 * 1867 / 723 + 1954 - 1496 * 1479 = -2210220
```

---
## Hope you will enjoy your miniprocessor
