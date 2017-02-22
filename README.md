## **Mini Matching Engine**
This is a mini trading order matching engine for Unix like platform.  
Matching rule is simple: the earlier sent order will be traded earlier if the prices could be matched (buy price not lower than sell price).  
Unit test is handled by Makefile. 

#### **Requirements:**
* C++11
* clang 3.5 / gcc 4.8 or newer

#### **Compiling And Testing:**
To compile  
`$ make`

To run all unit test cases  
`$ make test`

Unit test cases for functionality of CANCEL, PRINT, MODIFY  
`$ make testcancel`  
`$ make testprint`  
`$ make testmodify`  
`$ make testbuy`  

#### **Supported Commands:**

Could take 7 kinds of input from stdin, formats listed below:

1, BUY IOC.  
2, BUY GFD.  
3, SELL IOC.  
4, SELL GFD.  
5, CANCEL order\_id, e.g. 'CANCEL 5z81f72a'.  
6, MODIFY.  
7, PRINT, no other parameter, just 'PRINT'. 

IOC order (??) will not be added to sell/buy list if it was not completely traded.  
GFD order (Good For Day) will be added to sell/buy list if it was not completely traded. 

#### License:
See the [License](https://github.com/luo4neck/MatchingEngine/blob/master/LICENSE) file for details. 

**Jan 2017 Dublin Ireland**
