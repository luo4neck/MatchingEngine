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

Could take 5 kinds of input from stdin, formats listed below:

1, BUY.  
2, SELL.  
3, CANCEL order\_id, e.g. 'CANCEL 5z81f72a'.  
4, MODIFY.  
5, PRINT, no other parameter, just 'PRINT'. 

Order could be partially traded or completely traded. Once a pair of orders (one sell and one buy) got traded, a message will be printed like the following format.  
`TRADE s151h23 1000 20 7sdfa91 1002 20`    
There are 2 order types, IOC and GFD:  
IOC order (Immediate or Cancel) will not be added to sell/buy list if it was not completely traded.  
GFD order (Good For Day) will be added to sell/buy list if it was not completely traded. 

#### License:
See the [License](https://github.com/luo4neck/MatchingEngine/blob/master/LICENSE) file for details. 

**Jan 2017 @Dublin Ireland**
