#include <bits/stdc++.h>
using namespace std;

//Can replace array by vectors everywhere for extra overhead of about 14% time


#define MOD 163577857
#define g 23
#define g_inv 28448323
#define MAXN 1 << 20
//998244353 = 1 + 7 x 17 x 2^23
/*
If p = c 2^k + 1 then g^c is a 2^k -th root of unity in the MOD modulo ring.
Here g = 3 
*/

long long int fact[MAXN], inv_fact[MAXN];
int coeffs[1+MAXN] = {0};

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

void init(){
	fact[0] = fact[1] = 1;
	for(int i = 2; i < MAXN; i++)
		fact[i] = (fact[i-1]*i)%MOD;

	inv_fact[0] = 1;
	inv_fact[MAXN-1] = raise(fact[MAXN-1],MOD-2);
	for(int i = MAXN-1; i > 0; i--){
		inv_fact[i-1] = (inv_fact[i]*i)%MOD;
		if(inv_fact[i-1] < 0) inv_fact[i-1] += MOD;
	}
}

long long int binom(int n,int r){
	long long int ans = (fact[n]*inv_fact[n-r])%MOD;
	ans = (ans*inv_fact[r])%MOD;
	return ans;
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


int multiply2(int* a, int* b, int n1, int n2){
	int n = 1;
	while(n < n1 + n2 - 1)
		n <<= 1;
	for(int i = n1; i < n; i++)
		a[i] = 0;
	for(int i = n2; i < n; i++)
		b[i] = 0;
	ntt(a,n,false);
	ntt(b,n,false);
	for(int i = 0; i < n; i++){
		a[i] = (1LL*a[i]*b[i])%MOD;
	}
	ntt(a,n,true);
}


void modifycoeff(int nn){
	//for now lets assume n is a power of 2
	//n is number of terms, i.e. degree+1
	if(nn == 1){
		//no terms multiplied
		coeffs[0] = coeffs[1] = 1;
		return;
	}
	if(nn%2){
		modifycoeff(nn-1);
		//cout << nn << endl;
		int binomial[2*nn];
		binomial[0] = nn;
		binomial[1] = 1;
		multiply2(coeffs,binomial,nn,2);
		return;
	}
	modifycoeff(nn/2);
	//cout << nn << endl;
	int N = nn/2;
	//int auxpol[2*nn], holder[2*nn], nextaux[2*nn]; //will hold P(x+N) coeffs
	int *auxpol = new int[2*nn], *holder = new int[2*nn], *nextaux = new int[2*nn];
	//cout << nn << endl;
	for(int i = 0; i <= N; i++){
		auxpol[i] = (int)((1LL*coeffs[N-i]*fact[N-i])%MOD);
	}
	nextaux[0] = 1;
	long long int Npow = 1;
	for(int i = 1; i <= N; i++){
		Npow = (Npow*N)%MOD;
		//cout << inv_fact[i] << endl;
		nextaux[i] = (int)((Npow*inv_fact[i])%MOD);
	}
	//for(int i = 0; i < n; i++){
	//	cout << nextaux[i] << ' ' << auxpol[i] << endl;
	//}
	multiply2(auxpol,nextaux,N+1,N+1);
	for(int i = 0; i <= N; i++){
		holder[i] = (int)((1LL*auxpol[N-i]*inv_fact[i])%MOD);
	}
	multiply2(coeffs,holder,N+1,N+1);
}

void solve(){
	int N, K;
	cin >> N >> K;
	for(int i = 0; i <= N; i++)
		coeffs[i] = 0;
	modifycoeff(N);
	long long int sum = 0, term;
	for(int r = 1; r <= K; r++){
		term = binom(K-1,r-1);
		term = (term*coeffs[N-r])%MOD;
		sum = (sum+term)%MOD;
	}
	cout << sum << endl;
}


int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	init();
	int T;
	cin >> T;
	while(T--)
		solve();
	return 0;
}