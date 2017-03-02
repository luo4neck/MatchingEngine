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

int main(int argc, char** argv)
{
	cout<<"test"<<endl;

	if( argc != 2 )
	{
		cout<<"this script can only take 1 parameter"<<endl;
		return 1;
	}

	auto total = stoi( argv[1] );
	cout<<"total orders->"<<total<<endl;

	random_device rd;
	mt19937 gen( rd() );

	uniform_int_distribution<int> price_dist(MIN_PRICE, MAX_PRICE);
	uniform_int_distribution<int> quant_dist(MIN_QUANT, MAX_QUANT);
	uniform_real_distribution<double> oper_dist(0, 1); // to decide operation..

	if(total < 1) total = 100; // set to 100 if not set..

	for(size_t i=0; i<total; i++)
	{
		// switch()
		// buy, sell, modbuy, modsell, cancel, print

		if(i%2 == 0)	cout<<"SELL ";
		else			cout<<"BUY ";

		cout<<"GFD ";

		cout<<price_dist( gen )<<" ";
		cout<<quant_dist( gen )<<" ";
		cout<<"order"<<i<<endl;
	}
	
	cout<<"PRINT"<<endl;

	return 0;
}
