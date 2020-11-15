#include <filesystem>
#include <string>
#include <vector>

#include <opencv4/opencv2/highgui.hpp>

#include "MRIFile.cpp"

template <typename Storage>
class MRIStorageIterator{
    public:
        using ValueType = typename Storage::ValueType;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;

    public:
        MRIStorageIterator(PointerType file): m_Ptr(file) {};
        MRIStorageIterator& operator++(){
            m_Ptr++;
            return *this;
        };

        MRIStorageIterator operator++(int){
            MRIStorageIterator iterator = *this;
            ++(*this);
            return iterator;
        };

        MRIStorageIterator& operator--(){
            m_Ptr--;
            return *this;
        };

        MRIStorageIterator operator--(int){
            MRIStorageIterator iterator = *this;
            --(*this);
            return iterator;
        };

        ReferenceType operator[](int index){
            return *(m_Ptr + index);
        };
        PointerType operator->(){
            return m_Ptr;
        };

        ReferenceType operator*(){
            return *m_Ptr;
        }

        bool operator==(const MRIStorageIterator<Storage>& other) const{
            return m_Ptr == other.m_Ptr;
        }

        bool operator!=(const MRIStorageIterator<Storage>& other) const{
            return !(*this == other);
        }


    private:
        PointerType m_Ptr;
};


template<class T>
class MRIStorage{
    private:
        typename std::vector<T> _data;

    public:
        using Iterator = typename std::vector<T>::iterator;

        Iterator begin() {return _data.begin();}
        Iterator end() {return _data.end();};
        void load_data(std::string dir_path);
};

template<class T>
void MRIStorage<T>::load_data(std::string dir_path){
    std::filesystem::path path {dir_path};

    if(std::filesystem::exists(path)){
        for(std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)){
            T tmp {cv::imread(entry.path())};
            tmp.filename = entry.path();
            _data.push_back(tmp);
        }
    }
}