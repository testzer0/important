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

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	string S,R;
	cin >> S >> R;
	int s = S.length(), r = R.length();
	cd A[2*(s+r)] = {0}, Arev[2*(s+r)] = {0};
	cd C[2*(s+r)] = {0}, Crev[2*(s+r)] = {0};
	cd T[2*(s+r)] = {0}, Trev[2*(s+r)] = {0};
	cd G[2*(s+r)] = {0}, Grev[2*(s+r)] = {0};
	for(int i = 0; i < s; i++){
		switch(S[i]){
			case 'A':
				A[i] = 1;
				break;
			case 'C':
				C[i] = 1;
				break;
			case 'T':
				T[i] = 1;
				break;
			case 'G':
				G[i] = 1;
		}
	}
	for(int i = 0; i < r; i++){
		switch(R[i]){
			case 'A':
				Arev[r-1-i] = 1;
				break;
			case 'C':
				Crev[r-1-i] = 1;
				break;
			case 'T':
				Trev[r-1-i] = 1;
				break;
			case 'G':
				Grev[r-1-i] = 1;
		}
	}

	//now multiply the polynomials
	multiply(A,Arev,s,r);
	multiply(C,Crev,s,r);
	multiply(T,Trev,s,r);
	multiply(G,Grev,s,r);

	//say while comparing i-shift of S with R, sum S[i+x] R[x] = sum a[i+x] arev[r-1-x] = coeff of r-1+i
	//also compare only upto s-r shift
	int maxv = 0;
	for(int i = 0; i <= s-r; i++){
		//cout << maxv << endl;
		maxv = max(maxv, (int)(A[r-1+i].real()+C[r-1+i].real()+T[r-1+i].real()+G[r-1+i].real()));
	}
	maxv = r - maxv;
	cout << maxv << endl;

	return 0;
}