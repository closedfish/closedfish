#pragma once
//#include "C:\Users\Cassi\Downloads\eigen-3.4.0\eigen-3.4.0\Eigen\Dense"
#include <Eigen\Dense>


/*
*@brief This is the funtion class that will enables us to have an array of functions without to many technical difficulties.
*
*The class contains the following methods:
*
*   - InitFunc: this will dermine what function of the class will be chosen. The following are the possible functions:
*
*       - the square distance function of a distance
*       - a modified erf function computed on the absolute value of a difference
*       - a modified erf function computed on the square of a difference
*       - a modified arctan function computed on the absolute value of a difference
*       - a modified arctan function computed on the square of a difference
*
*   - Eval: Evaluates the function on a given chess board board. The 
*   
*
*/

class Func
{
    public:

        Func()
        {

        }

        /*
        *@brief This method Inititialises the function we choose
        *
        *@param func_num: the function it will choose (possibilities are described above)
        *@param  position_to_consider: all the function consider to specific pons out of the 16 provided (except the average function for which this parameter isn't used)
        *@param difference_type: are we computing a difference on pons of the same color or different color
        *@param height: in the case it is same color this tells us if we are considering the top or bottom pons
        */

        void InitFunc(int func_num, int position_to_consider, int difference_type, int height) 
        { 
            this->func_num = func_num; 
            placement = position_to_consider;
            this->difference_type = difference_type;
            this->height = height;//2 for top, 1 for bottom
            

        }


        /*
        *@brief Evaluate the function on a board (because we only consider the pons on the board here, l_top_pons and l_bottom_pons fully describe the board)
        *@param l_top_pons: the pons of the player at the top of the board
        *@param l_bottom_pons: the pons of the player at the bottom of the board
        *@return the actual computated value of the function
        */
        float Eval(int* l_top_pons, int* l_bottom_pons)
        {
            if (difference_type == 1){
                if (height == 2){
                    return Eval_help(l_top_pons);
                }
                else
                {
                    return Eval_help(l_bottom_pons);
                }
            }
            else
            {
                return Eval_help(l_top_pons, l_bottom_pons);
            }
        }

        /*
        *@brief helper function for Evaluate in the case where we are considering difference between pons heights of pons of distinct color
        *@param l_top_pons: the pons of the player at the top of the board
        *@param l_bottom_pons: the pons of the player at the bottom of the board
        *@return the actual computated value of the function
        */
        float Eval_help(int* l_top_pons, int* l_bottom_pons)
        {
            int t = l_top_pons[placement];
            int b = l_bottom_pons[placement];

            if (func_num == 0)
            {
                return ceil(100 * AveragePos(l_top_pons, l_bottom_pons)) / 100;
            }

            if (func_num == 1)
            {
                return SquareDistance(t, b);
            }
             else
            {
                return AbsoluteErfDistance(t, b);
            }

           
        }

        /*
        *@brief helper function for Evaluate in the case where we are considering difference between pons heights of pons of same color
        *@param l: the pons could be pons of top or bottom player depending on initialization
        *@return the actual computated value of the function
        */
        float Eval_help(int* l)
        {
            int pon1 = l[placement];
            int pon2 = l[placement + 1];
            
            if (func_num == 1)
            {
                return SquareDistance(pon1, pon2);
            }
             else
            {
                return AbsoluteErfDistance(pon1, pon2);
            }
            

        }

        
        /*
        *@brief We consider here the square difference of two heights of two pons on the board
        *@return the square difference of two heights of two pons on the board
        */
        int SquareDistance(int x, int y)
        {
            return  (x - y) * (x - y);
        }

        /*
        *@brief Here we consider a modified erf function. We use erf because it is a function that varies slowly and then very rapidly.
        *@brief I modified it after some calculation so that is even more quickly (that's why there is the times 10) and is very close to zero for each difference less or equal to 1.
        *@return computation of the function
        */

        int AbsoluteErfDistance(int x, int y)
        {
            if (x > y)
            {
                return (erf(10 * ((x - y) - 1)) + 1) / 2;
            }

            else
            {
                return (erf(10 * ((y - x) - 1)) + 1)/2;
            }
        }

        /*
        *@brief this is to take into account that if on average the pons are shifted towards a certain side, the board should be less closed
        *@param l_top_pons: the pons of the player at the top of the board
        *@param l_bottom_pons: the pons of the player at the bottom of the board
        *@return the distance between the average position of all of the pons and the middle of the board (of height 3.5)
        */
        

        float AveragePos(int* l_top_pons, int* l_bottom_pons)
        {
            int tot_sum = 0;
            double pon_count = 0;

            for (int i = 0; i < 8; i ++){

                int pos_1 = l_top_pons[i];
                int pos_2 = l_bottom_pons[i];
                if (pos_1 <= 7)
                {
                    tot_sum += pos_1;
                    pon_count += 1;
                }
            
                if (pos_2 >= 0)
                {
                    tot_sum += pos_2;
                    pon_count += 1;
                }
            }

            
            float ret = (float)tot_sum / (float) pon_count;

            if (ret > 3.5)
            {
                return ret - 3.5;
            }
            return 3.5 - ret;
        }


    private:

        int func_num;
        int placement;
        int difference_type;
        int height;
          
};      




namespace TheRegression {
    
    //This function creates the matrix for the regression from the function basis Basis (a pointer) and dimension size of the basis.
    //we have that the datapoint coordinates are stored in the pointer X of a list of length num_data_points 
    
    Eigen::MatrixXd setUpQ(Func* basis, int** X, int dimension, int num_data_points)
    {
        Eigen::MatrixXd Q(num_data_points, dimension);



        for (int j = 0; j < dimension; j++)
        {
            for (int i = 0; i < num_data_points; i++)
            {
                Q(i, j) = basis[j].Eval(X[2 * i], X[2 * i + 1]);
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

    Eigen::VectorXd bestFitF(Func* basis, int** X, double* data_outputs, int dimension, int num_data_points){

        Eigen::MatrixXd Q = setUpQ(basis, X, dimension, num_data_points);
        Eigen::VectorXd Y = setUpYVect(data_outputs, num_data_points);

        Eigen::MatrixXd trans_Q = Q.transpose();
        Eigen::MatrixXd mu(23, 23);
        mu.setIdentity();
        Eigen::MatrixXd helper_M = (trans_Q * Q + mu * 0.001).inverse();

        return (helper_M * trans_Q) * Y;

    }



}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

namespace SqrtDifBasis
{
    Func* GenerateBasis()
    {
        Func* basis = new Func[23]; 

        for (int i = 0; i < 7; i ++)
        {
            basis[i].InitFunc(1, i, 1, 2);//top pons consecutive difference
            basis[i + 7].InitFunc(1, i, 1, 1);//bottom pons consecutive difference
        }
        for (int j = 0; j < 8; j ++)
        {
            basis[j + 14].InitFunc(1, j, 2, -1);
        }
        
        basis[22].InitFunc(0, -1, 2, -1);

        return basis;

    }

}


namespace AbsErfBasis
{
      Func* GenerateBasis()
    {
        Func* basis = new Func[23]; 

        for (int i = 0; i < 7; i ++)
        {
            basis[i].InitFunc(2, i, 1, 2);//top pons consecutive difference
            basis[i + 7].InitFunc(2, i, 1, 1);//bottom pons consecutive difference
        }
        for (int j = 0; j < 8; j ++)
        {
            basis[j + 14].InitFunc(2, j, 2, -1);
        }
        
        basis[22].InitFunc(0, -1, 2, -1);

        return basis;
    }

}


namespace AbsSqrtDifBasis{
     Func* GenerateBasis()
    {
        Func* basis = new Func[23]; 

        for (int i = 0; i < 7; i ++)
        {
            basis[i].InitFunc(2, i, 1, 2);//top pons consecutive difference
            basis[i + 7].InitFunc(2, i, 1, 1);//bottom pons consecutive difference
        }
        for (int j = 0; j < 8; j ++)
        {
            basis[j + 14].InitFunc(1, j, 2, -1);
        }
        
        basis[22].InitFunc(0, -1, 2, -1);

        return basis;

    }

}


namespace SqrtAbsDifBasis
{
     Func* GenerateBasis()
    {
        Func* basis = new Func[23]; 

        for (int i = 0; i < 7; i ++)
        {
            basis[i].InitFunc(1, i, 1, 2);//top pons consecutive difference
            basis[i + 7].InitFunc(1, i, 1, 1);//bottom pons consecutive difference
        }
        for (int j = 0; j < 8; j ++)
        {
            basis[j + 14].InitFunc(2, j, 2, -1);
        }
        
        basis[22].InitFunc(0, -1, 2, -1);

        return basis;

    }
}



namespace EvaluationFunction
{   
    

    float Evaluate(Func* basis, Eigen::VectorXd theta, int* l_top_pons, int* l_bottom_pons, int dimension)
    {
        float output_val = 0;
        for (int i = 0; i < dimension; i++) 
        {
            output_val += basis[i].Eval(l_top_pons, l_bottom_pons) * theta[i];
        }
        if (output_val >= 1)
        {
            return 1;
        }
        
        return output_val;
    }

    float TestAi(Func* basis, Eigen::VectorXd theta, int** test_data_points, double* outputs, int dimension, int num_data_points)
    {
        float emp_risk = 0;

        for (int i = 0; i < num_data_points; i++)
            {
                emp_risk += pow(outputs[i] - Evaluate(basis, theta, test_data_points[2 * i], test_data_points[2 * i + 1], dimension), 2);
            }
        
        return emp_risk / (float)num_data_points;
    }
}