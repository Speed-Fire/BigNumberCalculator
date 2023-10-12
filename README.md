@mainpage Calculator

**Author:** Vladislav Sidorovici

**Date:** 22.05.2022

## Calculator

This application allows you to compute math expressions with operands of unlimited length.
Also you can save results of math expressions to variables and use these variables in next expressions.
Besides, the history of your actions can be loaded or saved to the file.

Calculator understands a several commands:
 > load - load calcultor history from the file, if it exists;
 
 > save - save current calcultor history (including variables) to file;
 
 > cls - clear the whole screen;
 
 > clv - clear all existing variables;
 
 > clh - clear current history (not history file);
 
 > test - start tests from file examples.tst;
 
 > exit - shut down the application;
 
 All another expressions will be considered as math expressions.

Available math actions:
<pre>
 1. adition (any numbers)                       - +;
 2. subtraction (any numbers)                   - -;
 3. multiplication (any numbers)                - *;
 4. Division (only integer numbers)             - /;
 5. Reminder of division (only integer numbers) - %.
 </pre>
 
 If after action over two fractional numbers you get integer number, you can use on it any of described upper math actions.

There are two inbuilt constants: pi and e.

 > pi = 3.1415926535897932
 
 > e  = 2.7182818284590452
 
To make a variable your expressions should look like:

 > _var_name = _expression
 
 There are some limits for variable names:
  1. the name can't be the same as commands, already existing variable, constant;
  
  2. the name must be only one word;
  
  3. the name can contains only letters and ciphers;
  
  4. the name can't start with a cipher.  
  
When you're writing a fractional number, you have to have on mind, that calcultor doesn't understand commas, only dots:

 > 123,45 - error
 
 > 123.45 - success