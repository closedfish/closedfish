#pragma once
#include "C:\Users\Cassi\Downloads\eigen-3.4.0\eigen-3.4.0\Eigen\Dense"


//This class enables us to have a list that contains function. It will be very useful for the different basis of function we will test
class Funk{
public:
    Funk(){ 
    }

    void choose_function(bool b)
    {
        i = b;
    }

    int eval(int* l_white_pons, int* l_black_pons, pos)
    {
        if (i){
            if l_white_pons[pos] > 0 && l_black_pons[pos] >0
                return square_distance(l_white_pons[pos], l_black_pons[pos]);
        }

        return absolute_distance(x, y);
    }
    int square_distance(int x, int y)
    {
        return  (x - y) * (x - y);
    }

    int absolute_distance(int x, int y)
    {
        if (x > y){
            return x - y;
        }

        else{
            return y - x;
        }
    }


    int average_pos(int* l_white_pons, int* l_black_pons)
    {
        int average = 0;
        double pon_count = 0;

        for (int i = 0; i < 8; i ++){
            int pos = l_white_pons[i];
            if (pos >= 0){
                average += pos;
                pon_count += 1;
                std::cout << pon_count << " " << average << std::endl;
            }
        }

        for (int i = 0; i < 8; i ++){
            int pos = l_black_pons[i];
            if (pos >= 0){
                average += pos;
                pon_count += 1;
                std::cout << pon_count << " " << average << std::endl;
            }
        }
        return 1.0 * average / pon_count;
    }
private:
    bool i;
};


namespace TheRegression {
    
    //This function creates the matrix for the regression from the function basis Basis (a pointer) and dimension size of the basis.
    //we have that the datapoint coordinates are stored in the pointer X of a list of length num_data_points 
    Eigen::MatrixXd setUpQ(Funk* Basis, double** X, int dimension, int num_data_points)
    {
        Eigen::MatrixXd Q(num_data_points, dimension);

        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < num_data_points; j++){

                Q(dimension, num_data_points) = Basis[i].square_distance(1, 2); 
            }
        }

        return Q;
    }

    //create the vector Y (the output obained for the data points)
    Eigen::VectorXd setUpYVect(double* data_outputs, int num_data_points){
        Eigen::VectorXd Y(num_data_points);
        for (int i = 0; i < num_data_points; i++){
            Y(i) = data_outputs[i];
        }

        return Y;
    }
    //this is the best linear combination of the basis functions to get the best fit in the form of 
    //a vector of length dimension
    //m here is the size of the vectors contained in X (14 <= m <= 16)
    Eigen::VectorXd bestFitF(Funk* Basis, double** X, double* data_outputs, int dimension, int num_data_points, int m){

        Eigen::MatrixXd Q = setUpQ(Basis, X, dimension, num_data_points);
        Eigen::VectorXd Y = setUpYVect(data_outputs, num_data_points);

        Eigen::MatrixXd trans_Q = Q.transpose();
        Eigen::MatrixXd helper_M = (trans_Q * Q).inverse();

        return helper_M * trans_Q * Y;

    }



}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

namespace Basis1{
    Funk* square_dist_basis(int* l_pons_white, int* l_pons_black)

}

namespace Basis2{

}

namespace Basis3{

}

