#include "otsp_solver.h"

OTSP_Solver::OTSP_Solver()
{
    data = false;
    pointsNumber = 0;
}

void OTSP_Solver::set_data(TSP_Table data)
{
    table = data;
    pointsNumber = table.size();
    path.clear();
    pathlen = 0.0;

    rows.resize(pointsNumber);
    cols.resize(pointsNumber);

    for(int i=0; i<pointsNumber; i++)
    {
        rows[i] = i;
        cols[i] = i;
    }

    data = true;
}

std::vector<int> OTSP_Solver::getPath()
{
   return path;
}
double OTSP_Solver::getPathLength()
{
    return pathlen;
}

TSP_Table OTSP_Solver::reduce_table_vals(TSP_Table &table)
{
    TSP_Table result = table;

    for(int i=0; i<pointsNumber; i++)
    {
        reduce_row_vals(result, i);
    }

    for(int i=0; i<pointsNumber; i++)
    {
        reduce_col_vals(result, i);
    }

    return result;
}
void OTSP_Solver::reduce_row_vals(TSP_Table &table, int index)
{
    double min_val = table[index][0];
    for(int j=1; j<pointsNumber; j++)
    {
        if(table[index][j]!=TSP_M)
        {
            if(table[index][j]<min_val)
                min_val = table[index][j];
        }
    }

    for(int j=0; j<pointsNumber; j++)
    {
        table[index][j] -= min_val;
    }
}
void OTSP_Solver::reduce_col_vals(TSP_Table &table, int index)
{
    double min_val = table[0][index];
    for(int i=0; i<pointsNumber; i++)
    {
        if(table[i][index]!=TSP_M)
        {
            if(table[i][index]<min_val)
                min_val = table[i][index];
        }
    }

    for(int i=0; i<pointsNumber; i++)
    {
        table[i][index] -= min_val;
    }
}

void OTSP_Solver::calculate_path()
{
    if(!data)
    {
        throw std::exception("NoData!");
    }

    TSP_Table work_table = reduce_table_vals(table);
    while(pointsNumber>1)
    {
        int i,j;
        std::tuple(i, j) = getTopRatedPoint(work_table);
        add_to_path(i, j);
        work_table[j][i] = TSP_M;
        reduce_table(work_table, i, j); //pointsNumber--; here

        if(pointsNumber>1)
            work_table = reduce_table_vals(work_table);
    }

    add_to_path(0,0);
}

void OTSP_Solver::add_to_path(int i, int j)
{
    int asb_i = rows[i];
    int abs_j = cols[j];

    //la first point costille
    if(abs_i != path[pointsNumber-1])
    {
        path.push_back(abs_i);
    }
    path.push_back(abs_j);

    pathlen += table[abs_i][abs_j];
}

void OTSP_Solver::reduce_table_vals(TSP_Table &table, int i, int j)
{
    table.erase(table.begin()+i);
    for(int t=0; t<(int)table.size(); t++)
    {
        table[t].erase(table[t].begin()+j);
    }

    pointsNumber--;
}
