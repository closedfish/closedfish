#pragma once
#include "C:\Users\Cassi\Downloads\eigen-3.4.0\eigen-3.4.0\Eigen\Dense"


//This class enables us to have a list that contains function. It will be very useful for the different basis of function we will test
class Funk
{
    public:

        Funk(int func, int position_to_consider)
        { 
            func_num = func; 
            placement = position_to_consider;
        }


        int eval(int* l_white_pons, int* l_black_pons)
        {
            int x = l_white_pons[placement];
            int y = l_black_pons[placement];

            if ((x < 0) || (y < 0))
            {
                return 0;
            }

            if (func_num = 0)
            {
                return average_pos(l_white_pons, l_black_pons);
            }

            if (func_num = 1)
            {
                return square_distance(x, y);
            }
             if (func_num = 2)
            {
                return square_distance(x, y);
            }

           
        }

        int eval(int* L)
        {
            int x = L[placement];
            int y = L[placement];
            
            if ((x < 0) || (y < 0))
            {
                return 0;
            }

            if (func_num = 1)
            {
                return square_distance(x, y);
            }
             if (func_num = 2)
            {
                return square_distance(x, y);
            }

        }


        int square_distance(int x, int y)
        {
            return  (x - y) * (x - y);
        }

        int absolute_distance(int x, int y)
        {
            if (x > y)
            {
                return x - y;
            }

            else
            {
                return y - x;
            }
        }


        float average_pos(int* l_white_pons, int* l_black_pons)
        {
            int average = 0;
            double pon_count = 0;

            for (int i = 0; i < 8; i ++){

                int pos_1 = l_white_pons[i];
                int pos_2 = l_black_pons[i];
                if (pos_1 >= 0)
                {
                    average += pos_1;
                    pon_count += 1;
                    std::cout << pon_count << " " << average << std::endl;
                }
            
                if (pos_2 >= 0)
                {
                    average += pos_2;
                    pon_count += 1;
                    std::cout << pon_count << " " << average << std::endl;
                }
            }

            
            return (float)average / pon_count;
        }

    private:
        int func_num;
        int placement;
        
        
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
   
}

namespace Basis2{

}

namespace Basis3{

}

