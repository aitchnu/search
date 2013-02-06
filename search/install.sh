#do Splitting
#mkdir Wiki_Split_Files
#cd Wiki_Split_Files
#split -b 100M --suffix-length=4 ../$1
#cd ..
#python quantizing.py "Wiki_Split_Files/"

#compile files
g++ -O3 -std=c++11 invIndex.cpp -w -o invIndex
