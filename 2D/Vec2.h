template <typename T>
class _Vec2
{
public:
    _Vec2()
    {}
    _Vec2(T x, T y)
        :
        x(x),
        y(y)
    {}
    _Vec2(const _Vec2& vect)
        :
        _Vec2(vect.x, vect.y)
    {}
    template <typename T2>
    explicit operator _Vec2<T2>() const
    {
        return{ (T2)x,(T2)y };
    }
    T   LenSq() const
    {
        return x * x + y * y;
    }
    T   Len() const
    {
        return sqrt(LenSq());
    }
    _Vec2& normalize()
    {
        const T length = Len();
        x /= length;
        y /= length;
        return *this;
    }
    _Vec2 getNormalized() const
    {
        _Vec2 norm = *this;
        norm.normalize();
        return norm;
    }
    T   operator*(const _Vec2& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }
    _Vec2 operator-() const
    {
        return _Vec2(-x, -y);
    }
    _Vec2& operator=(const _Vec2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }
    _Vec2& operator+=(const _Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    _Vec2& operator-=(const _Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    _Vec2 operator+(const _Vec2& rhs) const
    {
        return _Vec2(*this) += rhs;
    }
    _Vec2 operator-(const _Vec2& rhs) const
    {
        return _Vec2(*this) -= rhs;
    }
    _Vec2& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    _Vec2 operator*(const T& rhs) const
    {
        return _Vec2(*this) *= rhs;
    }
    _Vec2& operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }
    _Vec2 operator/(const T& rhs) const
    {
        return _Vec2(*this) /= rhs;
    }
    bool  operator==(const _Vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool  operator!=(const _Vec2& rhs) const
    {
        return !(*this == rhs);
    }
	bool operator < (const _Vec2& rhs) const
	{
		return x < rhs.x && y < rhs.y;
	}
	bool operator > (const _Vec2& rhs) const
	{
		return x > rhs.x && y > rhs.y;
	}
public:
    T x{};
    T y{};
};

typedef _Vec2<float> vec2;
typedef _Vec2<int>   ivec2;

