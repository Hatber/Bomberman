#ifndef __FORMAT__
#define __FORMAT__

#include <sstream>

class Format {
public:
	template<typename T>
	Format &operator <<(const T &x) { ss << x; return *this; }

	operator std::string() { return ss.str(); }

private:
    std::stringstream ss;
};


#endif // __FORMAT__
