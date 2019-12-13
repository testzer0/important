#include <bits/stdc++.h>
using namespace std;

typedef complex<double> cd;
const double PI = acos(-1);

//Can replace array by vectors everywhere for extra overhead of about 14% time


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

bool compare(pair<int,int> ax, pair<int,int> bx){
	if(ax.second == bx.second)
		return ax.first < bx.first;
	else return ax.second > bx.second;
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	string S;
	cin >> S;
	int N = S.length();
	cd a[4*N] = {0}, arev[4*N] = {0};
	cd b[4*N] = {0}, brev[4*N] = {0};
	cd c[4*N] = {0}, crev[4*N] = {0};
	for(int i = 0; i < N; i++){
		switch(S[i]){
			case 'a':
				a[i] = 1;
				break;
			case 'b':
				b[i] = 1;
				break;
			case 'c':
				c[i] = 1;
		}
	}
	//fill reversed arrays
	for(int i = 0; i < N; i++){
		arev[i] = a[N-1-i];
		brev[i] = b[N-1-i];
		crev[i] = c[N-1-i];
	}

	//multiply the polynomials
	multiply(a,arev,N,N);
	multiply(b,brev,N,N);
	multiply(c,crev,N,N);

	//note that sum a[i]*a[i+k] = sum arev[n-1-i]*a[i+k] = coeff of x^(n+k-1)
	//thus for k-th right shift value = a[n+k-1] + b[n+k-1] + c[n+k-1]
	pair<int,int> valarray[N];
	//first = shift, second = value
	//artifically make valarray[0].second = -1; since we dont want to consider that
	for(int i = 0; i < N; i++){
		valarray[i].first = i;
		valarray[i].second = a[N+i-1].real() + b[N+i-1].real() + c[N+i-1].real();
	}
	valarray[0].second = -1;
	std::sort(valarray,valarray+N,compare);
	int i = 0, refer = valarray[0].second;
	cout << refer << endl;
	while(i < N && valarray[i].second == refer){
		cout << valarray[i].first << ' ';
		i++;
	}
	cout << endl;
	return 0;
}