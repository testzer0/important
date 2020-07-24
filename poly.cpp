#include <bits/stdc++.h>
using namespace std;

static inline void pad(vector<long long int> &poly, int n) {
	while (poly.size() < n)
		poly.push_back(0);
}

static inline vector<int> bit_reversal_permutation(int n) {
	vector<int> a(n);
	for (int i = 1, j = 0; i < n; i++) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) 
			j ^= bit;
		j ^= bit;
		if (i < j) {
			a[i] = j, a[j] = i;
		}
	}
	return a;
}

const long long int mod = 998244353;
const int g = 3;
const long long int root = 565042129;
const int root_pow = 1 << 20;
const long long int root_inv = 950391366;
const int pw = 20;

#define num_inv(x) raise(x,mod-2)
long long int root_powers[root_pow];

long long int raise(long long int a, int e) {
	if (!a)
		return 0;
	else if (!e)
		return 1;
	long long int res = raise(a,e/2);
	res = (res*res)%mod;
	if (e & 1)
		res = (res*a)%mod;
	return res;
}

inline void normalize_in_place(vector<long long int> &poly);

void fft(vector<long long int> &a, vector<int> &brp, bool invert) {
	int n = a.size();
	for (int i = 0; i < n; i++) {
		if (i < brp[i])
			swap(a[i], a[brp[i]]);
	}

	for (int len = 2, l = 19; len <= n; len <<= 1, l--) {
		long long int wlen = invert? root_powers[root_pow - (1 << l)] : root_powers[1 << l];
		for (int i = 0; i < n; i += len) {
			long long int w = 1;
			for (int j = 0; j < len/2; j++) {
				long long int u = a[i+j], v = (a[i+j+len/2]*w)%mod;
				a[i+j] = (u+v < mod)? u+v : u+v-mod;
				a[i+j+len/2] = (u-v < 0)? u-v+mod : u-v;
				w = (w*wlen)%mod;
			}
		}
	}

	if (invert) {
		long long int inv = raise(n,mod-2);
		for (auto &x : a)
			x = (x*inv)%mod;
	}
}

vector<long long int> multiply(vector<long long int> const &a, vector<long long int> const &b) {
	vector<long long int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
	int n = 1;
	while (n < a.size() + b.size())
		n <<= 1;
	fa.resize(n);
	fb.resize(n);
	vector<int> brp = bit_reversal_permutation(n);
	fft(fa,brp,false);
	fft(fb,brp,false);

	for (int i = 0; i < n; i++)
		fa[i] = (fa[i]*fb[i])%mod;
	fft(fa,brp,true);
	// normalize_in_place(fa); 
	// helps if you're doing heavy work like interpolation: uncomment if n >= 10k and you're doing stuff like multi pt eval/interpolation
	return fa;
}

void fill() {
	root_powers[0] = 1;
	for (int i = 1; i < root_pow; i++)
		root_powers[i] = (root_powers[i-1]*root)%mod;
}

// polynomial operations; a poly is assumed as a vector of long long int

inline void normalize_in_place(vector<long long int> &poly) {
	while (poly.size() && poly.back() == 0)
		poly.pop_back();
}

inline vector<long long int> normalize(vector<long long int> poly) {
	vector<long long int> p(poly);
	while (p.size() && p.back() == 0)
		p.pop_back();
	return p;
}

inline void derivative_in_place(vector<long long int> &poly) {
	for (int i = 1; i < poly.size(); i++) 
		poly[i-1] = (poly[i]*i)%mod;
	if (poly.size())
		poly.resize(poly.size()-1);
}

inline vector<long long int> derivative(vector<long long int> poly) {
	vector<long long int> d;
	for (int i = 1; i < poly.size(); i++) 
		d.push_back((poly[i]*i)%mod);
	return d;
}

inline void add_to(vector<long long int> p1, vector<long long int> p2) {
	int l = p2.size();
	if (l > p1.size())
		p1.resize(l);
	for(int i = 0; i < l; i++)
		p1[i] = (p1[i]+p2[i])%mod;
}

inline vector<long long int> add_return(vector<long long int> p1, vector<long long int> p2) {
	int l = p2.size();
	vector<long long int> p(p1.size());
	if (l > p1.size()) {
		p.resize(l);
		p1.resize(l);
	}
	else
		p2.resize(p1.size());
	for(int i = 0; i < p.size(); i++)
		p[i] = (p1[i]+p2[i])%mod;
	return p;
}

inline vector<long long int> sub_from(vector<long long int> p1, vector<long long int> p2) {
	int l = p2.size();
	vector<long long int> p(p1.size());
	if (l > p1.size())
		p.resize(l);
	for(int i = 0; i < l; i++) {
		p[i] = (p1[i]-p2[i])%mod;
		while (p[i] < 0)
			p[i] += mod;
	}
	return p;
}

inline void sub_from_in_place(vector<long long int> &p1, vector<long long int> p2) {
	int l = p2.size();
	if (l > p1.size())
		p1.resize(l);
	for(int i = 0; i < l; i++) {
		p1[i] = (p1[i]-p2[i])%mod;
		while (p1[i] < 0)
			p1[i] += mod;
	}
}

inline void mul_const(vector<long long int> &p, long long int c) {
	for (int i = 0; i < p.size(); i++)
		p[i] = (p[i]*c)%mod;
}

inline vector<long long int> mul_const_to(vector<long long int> p, long long int c) {
	vector<long long int> q(p.size());
	for (int i = 0; i < p.size(); i++)
		q[i] = (p[i]*c)%mod;
	return q;
}


inline vector<long long int> mod_xk(vector<long long int> p, int k) {
	// calculates p(x)%(x^k)
	return vector<long long int>(p.begin(), p.begin()+min(k,(int)p.size()));
}

inline vector<long long int> mul_xk(vector<long long int> p, int k) {
	// multiplies p by x^k
	vector<long long int> pk(p.size()+k);
	int i = 0;
	while (i < k)
		pk[i++] = 0;
	while (i < pk.size()) 
		pk[i++] = p[i-k];
	return pk;
}

inline vector<long long int> div_xk(vector<long long int> p, int k) {
	// returns p/x^k
	int n = p.size();
	k = min(k, n);
	return vector<long long int>(p.begin()+k,p.end());
}

inline vector<long long int> substr(vector<long long int> p, int l, int r) {
	int n = p.size();
	l = min(l, n);
	r = min(r, n);
	return vector<long long int>(p.begin()+l,p.begin()+r);
}

vector<long long int> inverse(vector<long long int> p, int k) {
	// computes the inverse series of p (i.e. p^-1) mod x^k
	vector<long long int> ans;
	ans.push_back(num_inv(p[0]));
	int a = 1;
	while (a < k) {
		vector<long long int> temp = substr(multiply(ans,mod_xk(p,2*a)), a, 2*a);
		sub_from_in_place(ans, mul_xk(mod_xk(multiply(ans, temp), a), a));
		a <<= 1;
	}
	return mod_xk(ans,k);
}

vector<long long int> reverse(vector<long long int> p, int n, bool rev = false) {
	vector<long long int> v(p);
	if (rev)
		v.resize(max(n, (int)v.size()));
	std::reverse(v.begin(), v.end());
	return mod_xk(v, n);
}

const int thresh = 500; // max length to run slow division

pair<vector<long long int>, vector<long long int>> divmod_slow(vector<long long int> p1, vector<long long int> p2) {
	// "Slow" division. Should be faster for small n.
	// The usual division algorithm. Avoids complexity of reversal for small n.
	vector<long long int> rem(p1), qu;
	while (rem.size() >= p2.size()) {
		qu.push_back((rem.back() * num_inv(p2.back()))%mod);
		if (qu.back()) {
			for (int i = 0; i < p2.size(); i++) {
				rem[rem.size() - 1 - i] -= (qu.back() * p2[p2.size() - 1 - i])%mod;
				while (rem[rem.size() - 1 - i] < 0)
					rem[rem.size()-1-i] += mod;
			}
		}
		rem.pop_back();
	}
	std::reverse(qu.begin(), qu.end());
	return make_pair(qu,rem);
}

pair<vector<long long int>, vector<long long int>> divmod(vector<long long int> p1, vector<long long int> p2) {
	// Division using reversal
	if (p1.size() < p2.size()) {
		return make_pair(vector<long long int>(), vector<long long int>(p1));
	}
	int diff = p1.size() - p2.size();
	if (min(diff, (int)p2.size()) < thresh)
		return divmod_slow(p1,p2);
	vector<long long int> D = reverse(mod_xk(multiply(reverse(p1, diff+1), inverse(reverse(p2, diff+1), diff+1)), diff+1), diff+1, true);
	vector<long long int> X = multiply(D, p2);
	normalize(X);
	vector<long long int> R = sub_from(p1, X);
	normalize(R);
	return make_pair(D,R);
}

vector<long long int> integrate(vector<long long int> poly) {
	// calculates integral of given polynomial
	vector<long long int> integral;
	integral.push_back(0);
	for (int i = 1; i <= poly.size(); i++) {
		integral.push_back((poly[i-1] * num_inv(i))%mod);
	}
	return integral;
}

vector<long long int> log(vector<long long int> poly, int n) {
	// calculates log of poly mod x^n
	assert(poly[0] == 1);
	return mod_xk(integrate(multiply(mod_xk(derivative(poly), n), inverse(poly, n))), n);
}

vector<long long int> exp(vector<long long int> poly, int n) {
	// calculates e^P(x) power series mod x^n
	if (poly.empty())
		return vector<long long int>({1});
	vector<long long int> ans({1});
	int a = 1;
	while (a < n) {
		vector<long long int> C = sub_from(div_xk(log(ans, 2*a), a), substr(poly, a, 2*a));
		sub_from_in_place(ans, mul_xk(mod_xk(multiply(ans, C), a), a));
		a <<= 1;
	}
	return mod_xk(ans, n);
}

int leading_xk(vector<long long int> poly) {
	if (poly.empty())
		return 1e9;
	int i = 0;
	while (i < poly.size()) {
		if (poly[i])
			return i;
		i++;
	}
	return 1e9;
}

vector<long long int> pow_slow(vector<long long int> poly, int k, int n) {
	if (!k)
		return vector<long long int>({1});
	vector<long long int> pw = pow_slow(poly, k/2, n);
	return (k&1)? mod_xk(multiply(mod_xk(multiply(pw, pw), n), poly), n) : mod_xk(multiply(pw, pw), n);
}

vector<long long int> pow(vector<long long int> poly, int k, int n) {
	// calculate p(x)^k mod x^n
	if (poly.empty())
		return vector<long long int>();
	if (k < thresh)
		return pow_slow(poly,k, n);
	int i = leading_xk(poly);
	long long int j = poly[i];
	vector<long long int> t = div_xk(poly, i);
	mul_const(t, num_inv(j));
	vector<long long int> res = mod_xk(mul_xk(exp(mul_const_to(log(t, n), k), n), (i*k)), n);
	mul_const(res, raise(j, k));
	return res;
}

vector<long long int> mulx(vector<long long int> poly, long long int x) {
	// multiples coeff of <var>^k with x^k
	vector<long long int> p(poly.size());
	long long int cur = 1;
	for (int i = 0; i < poly.size(); i++) {
		p[i] = (poly[i] * cur)%mod;
		cur = (cur*x)%mod;
	}
	return p;
}

vector<long long int> mulx_sq(vector<long long int> poly, long long int x) {
	// multiplies coeff of <var>^k with x^{k^2}
	long long int cur = x, total = 1, xx = (x*x)%mod;
	vector<long long int> p(poly.size());
	for (int i = 0; i < poly.size(); i++) {
		p[i] = (poly[i] * total)%mod;
		total = (total*cur)%mod;
		cur = (cur*xx)%mod;
	}
	return p;
}

vector<long long int> chirpz_even(vector<long long int> poly, long long int z, int n) {
	// P(1), P(z^2), P(z^4), ..., P(z^(2*(n-1)))
	// poly should be normalized
	int m = poly.size() - 1;
	if (m == -1)
		return vector<long long int>(n,0);
	vector<long long int> vv(m+n);
	long long int zi = num_inv(z);
	long long int zz = (zi*zi)%mod;
	long long int cur = zi;
	long long int total = 1;
	for (int i = 0; i <= max(n-1, m); i++) {	// vv is the z^-(r-k)^2 array
		if (i <= m) 
			vv[m-i] = total;
		if (i < n)
			vv[m+i] = total;
		total = (total*cur)%mod;
		cur = (cur*zz)%mod;
	}
	// inner mul_sq sets up a_k z^k^2. The multiply makes sure that ans_2r is at coeff of r.
	// outer mul_sq ia the z^r^2 factor.
	vector<long long int> w = mulx_sq(substr((multiply(mulx_sq(poly, z), vv)), m, m+n), z);
	vector<long long int> res(n);
	for (int i = 0; i < n; i++) 	// copy step required: on shrinking a vector extra space is not freed/trimmed
		res[i] = w[i];
	return res;
}

vector<long long int> chirpz(vector<long long int> poly, long long int z, int n) {
	// P(1), P(z), ..., P(z^(n-1))
	auto even = chirpz_even(poly, z, (n+1)/2);
	auto odd = chirpz_even(mulx(poly, z), z, n/2);
	vector<long long int> ans(n);
	for (int i = 0; i < n/2; i++) {
		ans[2*i] = even[i];
		ans[2*i + 1] = odd[i];
	}
	if (n & 1)
		ans[n-1] = even.back();
	return ans;
}

#define polyrem(A, B) divmod(A,B).second

vector<long long int> eval(vector<long long int> poly, vector<long long int> x);

long long int eval(vector<long long int> poly, long long int x) {
	long long int res = 0;
	for (int i = poly.size() - 1; i >= 0; i--) {
		res = (res*x + poly[i])%mod;
	}
	return res;
}

vector<long long int> eval(vector<long long int> poly, vector<vector<long long int>> &tree, int v, vector<long long int>::iterator l, vector<long long int>::iterator r) {
	// evaluates polynomial at leaves of given tree (tree => evaluation tree for division)
	if (r-l == 1) 
		return {eval(poly, *l)};
	else {
		auto m = l + (r-l)/2;
		auto A = eval(polyrem(poly, tree[v << 1]), tree, v << 1, l, m);
		auto B = eval(polyrem(poly, tree[(v << 1)+1]), tree, (v << 1)+1, m, r);
		A.insert(A.end(), B.begin(), B.end());
		return A;
	}
}

vector<long long int> build(vector<vector<long long int>> &res, int v, vector<long long int>::iterator l, vector<long long int>::iterator r) {
	if (r - l == 1)
		return res[v] = vector<long long int>({mod - (*l), 1});
	else {
		auto m = l + (r-l)/2;
		return res[v] = multiply(build(res, v<<1, l, m), build(res, (v<<1)+1, m, r));
	}
}

vector<long long int> eval(vector<long long int> poly, vector<long long int> x) {
	int n = x.size();
	if (poly.empty())
		return vector<long long int>(n, 0);
	vector<vector<long long int>> tree(4*n);
	build(tree, 1, x.begin(), x.end());
	return eval(poly, tree, 1, x.begin(), x.end());
}


vector<long long int> inter(vector<long long int> poly, vector<vector<long long int>> &tree, int v, vector<long long int>::iterator l, vector<long long int>::iterator r, 
 vector<long long int>::iterator ly, vector<long long int>::iterator ry) {
	if (r-l == 1) 
		return {((*ly) * num_inv(poly[0]))%mod};
	else {
		auto m = l + (r-l)/2;
		auto my = ly + (ry-ly)/2;
		auto A = inter(polyrem(poly, tree[v<<1]), tree, v<<1, l, m, ly, my);
		auto B = inter(polyrem(poly, tree[(v<<1)+1]), tree, (v<<1)+1, m, r, my, ry);
		return add_return(multiply(A, tree[(v<<1)+1]), multiply(B, tree[v<<1]));
	}
}

vector<long long int> kmul(vector<long long int>::iterator l, vector<long long int>::iterator r) {
	// finds (x-a1)...(x-an) without building tree
	if (r-l == 1)
		return vector<long long int>({mod-(*l), 1});
	else {
		auto m = l + (r-l)/2;
		return multiply(kmul(l,m), kmul(m,r));
	}
}

vector<long long int> inter(vector<long long int> x, vector<long long int> y) {
	int n = x.size();
	vector<vector<long long int>> tree(4*n);
	return inter(derivative(build(tree, 1, x.begin(), x.end())), tree, 1, x.begin(), x.end(), y.begin(), y.end());
}

int main() {
	fill();
	vector<long long int> a(1000), b(1000);
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		a[i] = rand();
		b[i] = rand();
	}
	vector<long long int> c = inter(a, b);
	return 0;
}