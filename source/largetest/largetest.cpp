/* the code below could generate random orders for the matching engine
 * total number of orders is received as a parameter from ##
 * orders generated will be printed to stdout, then '## > ##.dat'
 * then will be used for testing..
 */
#include<iostream>
#include<random>

#define MIN_PRICE 1
#define MAX_PRICE 2000
#define MIN_QUANT 1
#define MAX_QUANT 100

using namespace std;

int main()
{
	default_random_engine price_engine;
	uniform_int_distribution<int> price_dist(MIN_PRICE, MAX_PRICE);

	default_random_engine quant_engine;
	uniform_int_distribution<int> quant_dist(MIN_QUANT, MAX_QUANT);

	const int total = 100; // should be replaced from input..

	for(size_t i=0; i<total; i++)
	{
		// switch()
		// buy, sell, modbuy, modsell, cancel, print

		if(i%2 == 0)	cout<<"SELL ";
		else			cout<<"BUY ";

		cout<<"GFD ";

		cout<<price_dist(price_engine)<<" ";
		cout<<quant_dist(quant_engine)<<" ";
		cout<<"order"<<i<<endl;
	}
	
	cout<<"PRINT"<<endl;

	return 0;
}
