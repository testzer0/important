#include <bits/stdc++.h>
using namespace std;

#define MOD 786433
#define g 10
#define g_inv 235930
#define wbase 1000
#define winv 710149
#define target 1 << 18

long long int raise(long long int a, long long int b){
	if(!a) return 0;
	if(!b) return 1;
	long long int result = raise(a,b/2);
	//if(result == 0) cout << a << ' ' << b << " " << result << endl;
	result = result*result %MOD;
	if(b & 1) result = result*a % MOD;
	return result;
}

int inverse(int a){
	long long int result = raise(a,MOD-2);
	return (int)result;
}

void ntt(int *a, int n, bool invert){
	//bit reversal
	for(int i = 1, j = 0; i <= n; i++){
		int bit = n >> 1; //flip leading 1's, bit represents bit to be flipped
		while(j & bit){
			j ^= bit;
			bit >>= 1;
		}
		//also flip first 0 to 1
		j ^= bit;
		//now j = rev(i)
		if(i < j){
			swap(a[i],a[j]);
		}
	}

	for(int len = 2; len <= n; len <<= 1){
		//we need wcur to be len-th root of unity
		int val = MOD-1;
		val /= len;
		int wcur = invert? (int) raise(g_inv,val) : (int)raise(g,val);
		//int wcur = (int) raise(g,val);
		//cout << wcur << endl;
		//cout << 1LL*wcur*wcur%MOD << endl;
		//if(invert) wcur = inverse(wcur);
		//cout << wcur << endl;
		for(int i = 0; i < n; i += len){
			int w = 1;
			for(int j = 0; j < len/2; j++){
				//cout << w << ' ';
				int u = a[i+j], v = (1LL*w*a[i+j+len/2])%MOD;
				a[i+j] = (int)((u + v)%MOD);
				if(a[i+j] < 0) a[i+j] += MOD;
				a[i+j+len/2] = (u-v);
				if(a[i+j+len/2] < 0) a[i+j+len/2] += MOD;
				w = (int)((1LL*w*wcur)%MOD);
				if(w < 0) w += MOD;			
			}
			//cout << endl;
		}
	}

	if(invert){
		int n_1 = inverse(n);
		for(int i = 0; i < n; i++)
			a[i] = (1LL*a[i]*n_1)%MOD;
	}

}


int main(){
	int N;
	cin >> N;
	const int n = 1 << 18;
	int coeffs1[n+100] = {0}, coeffs2[n+100] = {0}, coeffs3[n+100] = {0};
	for(int i = 0; i <= N; i++){
		cin >> coeffs1[i];
		coeffs2[i] = (int)((coeffs1[i]*raise(10,i))%MOD);
		coeffs3[i] = (int)((coeffs1[i]*raise(100,i))%MOD);
	}
	int mapping[MOD+1];
	mapping[0] = coeffs1[0];
	ntt(coeffs1,n,false);
	ntt(coeffs2,n,false);
	ntt(coeffs3,n,false);
	int w = 1;
	for(int i = 0; i < n; i++){
		if(i) w = (int)((1LL*1000*w)%MOD);
		mapping[w] = coeffs1[i];
		mapping[(int)((1LL*w*10)%MOD)] = coeffs2[i];
		mapping[(int)((1LL*w*100)%MOD)] = coeffs3[i];
	}
	int Q;
	cin >> Q;
	int pos;
	while(Q--){
		cin >> pos;
		cout << mapping[pos] << endl;
	}
	return 0;
}