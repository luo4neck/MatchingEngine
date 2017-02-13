### **Mini Matching Engine**
This is a mini trading order matching engine. Matching rule is simple: the earlier sent order will be traded earlier if the prices could be matched (buy price not lower than sell price).

#### **Requirements:**
clang 3.5 / gcc 4.8 or newer

#### **Compiling:**
$ make

#### **All unit tests running:**
$ make test


#### **Unit tests:**
$ make testcancel

$ make testprint

$ make testmodify

#### **Supported Commands:**

Could take 7 kinds of input from stdin:

1, BUY IOC. 

2, BUY GFD. 

3, SELL IOC. 

4, SELL GFD. 

5, CANCEL. 

6, MODIFY. 

7, PRINT. 

#### Sample of Input And Output
// todo

#### License..
// todo

##### Jan 2017 Dublin Ireland
