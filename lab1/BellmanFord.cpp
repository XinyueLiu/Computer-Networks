/*
 * To compile it:
 *    g++ -ggdb -Wall hello.cpp
 *
 * To run it:
 *    a.out
 */


// needed to do IO
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void  BellmanFord(int map[][5]){
	std::vector<int> tmpV;
	std::vector<int> routeV;
	string route = "";
	int i,j,tmp,m ;
	int imax =std::numeric_limits<int>::max();
	int preNode[5];
	int dPrev[5];
	int d[5] = {0,imax,imax,imax,imax};
	int iteration = 0;
	routeV.push_back(0);
	preNode[0] = 0;

	for(i=1; i<5; i++){
		iteration++;
		for(j=0; j<5; j++){
			dPrev[j] = d[j];
		}
		if(i > 1){
			for (std::vector<int>::iterator it = routeV.begin() ; it != routeV.end(); ++it){
				for(m=0; m<5; m++){
					if(map[*it][m] != imax && map[*it][m] != 0){
						tmpV.push_back(m);
					}
				}

			}
			routeV = tmpV;
			tmpV.erase (tmpV.begin(),tmpV.end());
		}

		// cout << "i:" << i << '\n';
		// for (std::vector<int>::iterator it = routeV.begin() ; it != routeV.end(); ++it){
		// 	cout << *it << ' ';
		// }
		// std::cout << '\n';

		for(m=1; m<5; m++){
			for (std::vector<int>::iterator it = routeV.begin() ; it != routeV.end(); ++it){
				if(map[*it][m] !=imax && d[m] > (d[*it]+map[*it][m])){
					d[m] = d[*it]+map[*it][m];
					preNode[m] = *it;
				}
			}
			
		}

		// for(tmp=0; tmp<5; tmp++){
		// 		cout<< preNode[tmp] << ' ';

		// }
		// cout<< '\n';


		for(j=0; j<5; j++){
			if(dPrev[j] != d[j]){
				break;
			}
		}
		if(j == 5) {
			for(tmp=0; tmp<4; tmp++){
				cout<< d[tmp] << ',';
			}
			cout<< d[tmp];
			cout<< '\n';

			for(tmp=0; tmp<5; tmp++){
				cout<< preNode[tmp] << ' ';
			}
			cout<< '\n';

			for(tmp=0; tmp<5; tmp++){
				m = preNode[tmp];
				std::string s;
				std::stringstream out;
				out.str("");
				out << tmp;
				route = out.str();
				while(m != 0){
					out.str("");
					out << m;
					route = out.str() + "->" + route;
					m = preNode[m];
				}
				if(tmp != 0){
					out.str("");
					out << 0;
					route = out.str() + "->" + route;
				}
				cout<<route<<'\n';

			}
			cout<<"Iteration:" << iteration << '\n';
			break; //chong fu le
		}
	}

}

int main(int argc, char * argv[]) {
  cout<< argc << '\n';
  int imax = std::numeric_limits<int>::max();
  int map[5][5] = {{0,1,imax,2,imax},
  				   {imax,0,4,2,imax},
  				   {imax,imax,0,imax,2},
  				   {imax,imax,4,0,2},
  				   {imax,imax,imax,imax,0}};
  BellmanFord(map);

  return 0;
}
