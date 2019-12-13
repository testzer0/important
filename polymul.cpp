#include<bits/stdc++.h>
using namespace std;

typedef complex<double> cd;
const double PI = acos(-1);

void fft(cd* a, int n, bool invert){
	/*-----------
	Computes FFT if invert is false, else inverse FFT
	i.e. coeffs -> values at the n roots of unity & vice versa
	it is assumed that n is a power of 2
	------------*/

	//apply bit reversal to move from rev(i) to rev(i+1)
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


	//now do fft iteratively for 2,4,8,..,n (n is a power of 2)
	for(int len  = 2; len <= n; len <<= 1){
		double angle = 2*PI / len;
		if(invert) angle = -angle;
		cd wcur(cos(angle),sin(angle));
		for(int i = 0; i < n; i += len){
			cd w(1,0); //start
			//for each stretch of length len, apply this level of fft
			for(int j = 0; j < len/2; j++){
				cd u = a[i+j], v = w*a[i+j+len/2];
				a[i+j] = u+v;
				a[i+j+len/2] = u-v;
				w *= wcur;
			}
		}
	}

	//if invert then divide overall by n
	if(invert){
		for(int i = 0; i < n; i++)
			a[i] /= n;
	}
}

int multiply(cd* a, cd* b, int n1, int n2){
	//multiplies a by b and stores the result in a;
	int n = 1;
	while(n < n1 + n2 - 1)
		n <<= 1;
	//a.resize(n); b.resize(n);
	for(int i = n1; i < n; i++)
		a[i] = 0;
	for(int i = n2; i < n; i++)
		b[i] = 0;
	fft(a,n,false);
	fft(b,n,false);
	for(int i = 0; i < n; i++){
		a[i] *= b[i];
	}
	fft(a,n,true);
	for(int i = 0; i < n; i++)
		a[i] = round(a[i].real());
	return n;
}

void solve(){
	int N;
	cin >> N;
	long long int temp;
	cd A[4*N], B[4*N];
	for(int i = 0; i <= N; i++){
		cin >> temp;
		A[i] = (cd)temp;
	}
	for(int i = 0; i <= N; i++){
		cin >> temp;
		B[i] = (cd)temp;
	}
	multiply(A,B,N+1,N+1);
	for(int i = 0; i <= 2*N; i++){
		cout << (long long int)A[i].real() << " ";
	}
	cout << endl;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int T;
	cin >> T;
	while(T--)
		solve();
	return 0;
}