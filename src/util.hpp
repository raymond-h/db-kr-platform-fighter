#ifndef __UTIL_HPP__
#define __UTIL_HPP__

template <typename It>
void printRange(const It &a, const It &b)
{
	cout << "[";
	bool printedFirstElem = false;
	for (auto i = a; i != b; i++)
	{
		if (printedFirstElem)
			cout << ", ";
		cout << *i;
		printedFirstElem = true;
	}
	cout << "]" << endl;
}

template <typename Range>
void printRange(const Range &r)
{
	printRange(std::begin(r), std::end(r));
}

#endif// __UTIL_HPP__
