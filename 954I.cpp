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
	cd a[2*(s+r)] = {0}, arev[2*(s+r)] = {0};
	cd b[2*(s+r)] = {0}, brev[2*(s+r)] = {0};
	cd c[2*(s+r)] = {0}, crev[2*(s+r)] = {0};
	cd d[2*(s+r)] = {0}, drev[2*(s+r)] = {0};
	cd e[2*(s+r)] = {0}, erev[2*(s+r)] = {0};
	cd f[2*(s+r)] = {0}, frev[2*(s+r)] = {0};
	for(int i = 0; i < s; i++){
		switch(S[i]){
			case 'a':
				a[i] = 1;
				break;
			case 'b':
				b[i] = 1;
				break;
			case 'c':
				c[i] = 1;
				break;
			case 'd':
				d[i] = 1;
				break;
			case 'e':
				e[i] = 1;
				break;
			case 'f':
				f[i] = 1;
		}
	}
	for(int i = 0; i < r; i++){
		switch(R[i]){
			case 'a':
				a[r-1-i] = 1;
				break;
			case 'b':
				b[r-1-i] = 1;
				break;
			case 'c':
				c[r-1-i] = 1;
				break;
			case 'd':
				d[r-1-i] = 1;
				break;
			case 'e':
				e[r-1-i] = 1;
				break;
			case 'f':
				f[r-1-i] = 1;
		}
	}

	//now multiply the polynomials
	multiply(a,arev,s,r);
	multiply(b,brev,s,r);
	multiply(c,crev,s,r);
	multiply(d,drev,s,r);
	multiply(e,erev,s,r);
	multiply(f,frev,s,r);

	//say while comparing i-shift of S with R, sum S[i+x] R[x] = sum a[i+x] arev[r-1-x] = coeff of r-1+i
	//also compare only upto s-r shift
	int cur;
	for(int i = 0; i <= s-r; i++){
		cur = 0;
		cur += a[r-1+i] + b[r-1+i] + c[r-1+i];
		cur += d[r-1+i] + e[r-1+i] + f[r-1+i];
		cout << cur < " ";
	}
	return 0;
}