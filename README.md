This is a C written console application for Windows. It is an 8086 assembly teacher. The user is given an exercise (so far there is only 1 exercise to choose from).
A code file is prompted with instructions. The user must follow the instructions and solve the exercise. Afterwards, the code is assembled using TASM and the application
checks for any errors in code. If the code is error-free, a test will be run. A test consists of a sequence of inputs and expected outputs. The user will be notified if
he passed all the cases or failed at least one.

The algorithm goes like that:
1) Let the user choose an exercise number.
Each exercise has its own folder, which is located at one of the folders at TASM/Exers
In each exercise folder there are 3 files - Code.asm, Test.asm and Code_Backup.asm - a deeper explaination is given later.

2) Let the user edit the code.
If he choses to edit, the Code.asm file, specific to the exercise, will be open.
The code file contains insructions the user must follow.

3) Check for assembling errors in user's code.
Before getting on to test the code, we must check if the code has errors.
To assemble, link and run the program, we must use an emulator, in this case, DOSBox 0.74-3.
The assembling is done using TASM - Turbo Assembler.
The linking is done by Turbo Linker.
Note that the user's code isn't actually being run, since it only has procedures in it and not a full program.
Before opening DOSBox, we configure its options such that it will Assemble the specific code file, based on the exercise number.
The configuration template file is named Configurations_Template.conf
To create the configuration file, we copy Configurations_Template.conf to a file named Configurations.conf
Then we append the auto-execute commands to Configurations.conf
Now, it is finally time to run DOSBox. We do it by running the batch file named Assemble_Code.bat.
DOSBox is run in the background. We achieved it by writing the command set sdl_videodriver=dummy beforehand.
The TASM output is written to a text file named TASM_OUT.txt 
We report the errors to the user.
If there are no errors we go on, else, we give the user a chance to edit the code.

4) Test the code
Here comes the role of Test.asm
For each exercise, there is a pre-made assembly program which tests the code.
Test.asm uses general functions from the file named Basic.asm at TASM\Exers.
The testing is done by running a number of pre-determined cases. 
Each case consists of input and expected output.
The assembling is done just like in 3)
The testing program always outputs a file named OUTPUT.txt at TASM folder.
Its content is 1 if all cases passed and 0 otherwise.
If the user code failed at least one test case, he is given a chance to edit the code
