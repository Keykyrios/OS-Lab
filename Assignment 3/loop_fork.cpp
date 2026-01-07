#include <iostream>
#include <unistd.h>
using namespace std;
int main() {
for (int i = 0; i < 4; i++)
{
fork();
cout<<"process"<<i<<endl;
}
// Each process will reach this point
return 0;
}
