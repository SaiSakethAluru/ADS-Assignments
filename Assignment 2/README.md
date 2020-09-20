### Instructions for running the code

1. Data generation - using the file `data_gen.cpp` <br>
`g++ -std=c++11 data_gen.cpp -o data_gen && ./data_gen`

2. Insertion to RTree and saving to file - using the files `insert.cpp` and `main.cpp` <br>
`g++ -std=c++11 main.cpp insert.cpp -o insert && ./insert`

3. Running Nearest neighbour search - using the file `NN_search.cpp` <br>
`g++ -std=c++11 NN_search.cpp -o nn_search && ./nn_search`

4. Running KNN search - using the file `KNN.cpp` <br>
`g++ -std=c++11 KNN.cpp -o knn && ./knn`