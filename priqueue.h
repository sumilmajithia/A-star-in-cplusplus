template<typename T>
class Heap 
{
	private:
	std::function<int(T&,T&)> cmpr;
	vector<T> tree;
	void Heapifyup (T a);
	void Heapifydown(T a);
	public:
	void insert(T a);
	T pop() ;
};

