#include <bits/stdc++.h>
using namespace std;

#define P 998244353
#define MAXN 1 << 20
#define g 3
#define ginv 332748118

long long int fact[MAXN], invfact[MAXN];
long long int gpow[30], invgpow[30];
int power = 24;

long long int pow_mod(long long int a, long long int b){
	if(b == 0) return 1;
	long long int result = pow_mod(a,b/2);
	result = (result*result);
	if(b%2) result *= a;
	result = result % P;
	return result;
}

long long int inv(long long int a){
	long long int in = pow_mod(a,P-2);
	return in;
}

void init(){
	//factorials
	fact[0] = fact[1] = 1;
	for(int i = 2; i < MAXN; i++)
		fact[i] = (fact[i-1]*i)%P;

	invfact[0] = 1;
	invfact[MAXN-1] = inv(fact[MAXN-1]);
	for(int i = MAXN-2; i > 0; i--){
		invfact[i] = (invfact[i+1]*(i+1))%P;
	}
	gpow[0] = invgpow[0] = 1;
	for(int i = 0; i < 30; i++){
		gpow[i] = (gpow[i-1]*g)%P;
		invgpow[i] = (invgpow[i-1]*ginv)%P;
	}
}

void ntt(vector<int> &a, bool invert){
	int n = a.size();
	for(int i = 1, j = 0; i < n; i++){
		int bit = n >> 1;
		for(; j & bit; bit >>= 1){
			j ^= bit;
		}
		j ^= bit;
		if(i < j) swap(a[i],a[j]);
	}
	for(int len = 2; len <= n; len <<= 1){
		int wlen = invert? ginv : g;
		for(int i = wlen; i < MAXN; i <<= 1){
			wlen = (int)(1LL*wlen*wlen%P);
		}

		for(int i = 0; i < n; i += len){
			int w = 1;
			for(int j = 0; j < len/2; j++){
				int u = a[i+j], v = (int)(1LL*a[i+j+len/2]*w)%P;
				a[i+j] = u + v < P? u+v : u+v-P;
				a[i+j+len/2] = u - v < 0? u - v + P : u-v;
				w = (int)(1LL*w * wlen % P); 
			}
		}
	}
	if(invert){
		int n1 = inv(n);
		for(int i = 0; i < n; i++){
			a[i] = (int) (a[i]*n1)%P;
		}
	}
}

void multiply(vector<int> & a, vector<int> & b) {
    int n = 1;
    while (n < a.size() + b.size() - 1) 
        n <<= 1;
    a.resize(n);
    for(int i = a.size(); i < n; i++)
    	a[i] = 0;
    b.resize(n);
    for(int i = b.size(); i < n; i++)
    	b[i] = 0;

    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; i++)
        a[i] = (long long)a[i]*b[i]%P;
    ntt(a, true);
}


int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	init();
	vector<int> a,b;
	a.push_back(2);
	a.push_back(3);
	b.push_back(1);
	b.push_back(1);
	multiply(a,b);
	for(auto & x : a) cout << x << endl;
	return 0;
}