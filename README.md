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
//todo take parameter from makefile for large scale test..  

#### **Supported Commands:**

Could take 5 kinds of input from stdin, formats listed below:

1, BUY  order\_type quantity price order\_id; e.g. 'BUY  GFD 300 32 u5d12t9'.  
2, SELL order\_type quantity price order\_id; e.g. 'SELL IOC 300 31 78wehyw'.  
3, CANCEL order\_id; e.g. 'CANCEL 5z81f72a'.  
4, MODIFY.  
5, PRINT; no other parameter.  

There are 2 order\_type, IOC and GFD:  
IOC order (Immediate or Cancel) will not be added to sell/buy list if it was not completely traded.  
GFD order (Good For Day) will be added to sell/buy list if it was not completely traded. 

Order could be partially traded or completely traded. Once a pair of orders (one sell and one buy) got traded, a message will be printed like the following format.  
`TRADE s151h23 1000 20 7sdfa91 1002 20`    

#### License:
See the [License](https://github.com/luo4neck/MatchingEngine/blob/master/LICENSE) file for details. 

**Jan 2017 @Dublin Ireland**
