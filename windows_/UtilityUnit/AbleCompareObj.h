
#ifndef _ABLE_COMPARE_OBEJCT_
#define _ABLE_COMPARE_OBEJCT_

class AbleCompareObject
{
public:
	virtual bool operator<(const AbleCompareObject& right)const = 0;
	bool operator> (const AbleCompareObject& right)const;
	bool operator>=(const AbleCompareObject& right)const;
	bool operator<=(const AbleCompareObject& right)const;
	bool operator==(const AbleCompareObject& right)const;
};

bool AbleCompareObject::operator>=(const AbleCompareObject& right)const
{
	return  !(*this < right);
}

bool AbleCompareObject::operator>(const AbleCompareObject &right)const
{
	return right < *this;
}

bool AbleCompareObject::operator<=(const AbleCompareObject &right)const
{
	return !(*this > right);
}

bool AbleCompareObject::operator==(const AbleCompareObject &right)const
{
	return !((*this<right) || (*this>right));
}

#endif
