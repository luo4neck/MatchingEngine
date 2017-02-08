Mini Matching Engine


Requirements:
clang 3.5 / gcc 4.8 or newer


Compiling:
$ make


All unit tests running:
$ make test


Unit tests:
$ make testcancel
$ make testprint
$ make testmodify


This is a mini trading order matching engine. 


Could take 7 kinds of input from stdin:

1, BUY IOC. 

2, BUY GFD. 

3, SELL IOC. 

4, SELL GFD. 

5, CANCEL. 

6, MODIFY. 

7, PRINT. 


Matching rule is simple: the ealier sent order will be traded ealier if the prices could be matched (buy price not lower than sell price). 


// sample of input and output..


// License..


Jan 2017 Dublin Ireland
