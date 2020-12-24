#include<vector>
#include<iostream>
using namespace std;

template <class T, class U>
void QuickSortCustomer(int start, int end, bool(*sort)(char*, char*), vector<U*>& t) {
	int left = start, right = end;
	if (left <= right) {
		char* pivot = t[(start + end) / 2]->getFullName();
		while (left <= right) {
			while ((*sort)(t[left]->getFullName(), pivot)) ++left;
			while (!(*sort)(t[right]->getFullName(), pivot)) --right;
			if (left < right) {
				U* temp1 = t[left];
				U* temp2 = t[right];
				t[left] = temp2;
				t[right] = temp1;
			}
			left++; right--;
		}
		if (start < right) QuickSortCustomer<T, U>(start, right, sort, t);
		if (left < end) QuickSortCustomer<T, U>(left, end, sort, t);
	}
}

template <class T, class U>
void QuickSortCategory(int start, int end, bool(*sort)(char*, char*), vector<U*>& t) {
	int left = start, right = end;
	if (left <= right) {
		char* pivot = t[(start + end) / 2]->getTenLoaiXe();
		while (left <= right) {
			while ((*sort)(t[left]->getTenLoaiXe(), pivot)) ++left;
			while (!(*sort)(t[right]->getTenLoaiXe(), pivot)) --right;
			if (left < right) {
				U* temp1 = t[left];
				U* temp2 = t[right];
				t[left] = temp2;
				t[right] = temp1;
			}
			left++; right--;
		}
		if (start < right) QuickSortCategory<T, U>(start, right, sort, t);
		if (left < end) QuickSortCategory<T, U>(left, end, sort, t);
	}
}