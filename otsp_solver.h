#ifndef OTSP_SOLVER_H
#define OTSP_SOLVER_H

#include<vector>
#include<tuple>
#include<exception>
#include<sstream>

typedef std::vector<std::vector<double> > TSP_Table;

namespace  TSP
{
    const double M = -1.0;
    const double UNDEFINED = -2.0;
}

class TSP_Exception: public std::exception {
private:
    std::string message;
public:
    explicit TSP_Exception(const std::string& message);
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class OTSP_Solver
{
public:
    OTSP_Solver();
    void set_data(TSP_Table data);
    void calculate_path();

    double getPathLength();
    std::vector<int> getPath();
    std::string path_to_string();
private:
    TSP_Table reduce_table_vals(TSP_Table &table);
    void reduce_row_vals(TSP_Table &table, int index);
    void reduce_col_vals(TSP_Table &table, int index);
    void reduce_table(TSP_Table &table, int i, int j);

    std::pair<int, int> getTopRatedPoint(TSP_Table &table);
    double getPointRating(TSP_Table &table, int r, int c);
    void add_to_path(int i, int j);

    bool data;
    int pointsNumber;
    TSP_Table table;
    std::vector<int> path;
    std::vector<int> rows, cols;
    double pathlen;
};

#endif // OTSP_SOLVER_H
