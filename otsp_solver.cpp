#include "otsp_solver.h"

TSP_Exception::TSP_Exception(const std::string& message)
{
    this->message = message;
}


OTSP_Solver::OTSP_Solver()
{
    data = false;
    pointsNumber = 0;
}

void OTSP_Solver::set_data(TSP_Table data_table)
{
    table = data_table;
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

std::string OTSP_Solver::path_to_string()
{
    if(path.empty()) return "Nothing";
    std::string  res;
    std::stringstream ss;
    ss<<path[0]+1;
    for(int i=1; i<(int)path.size(); i++)
    {
        ss<<"-->"<<path[i]+1;
    }

    ss>>res;
    return res;
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
    double min_val = TSP::UNDEFINED;

    for(int j=0; j<pointsNumber; j++)
    {
        if(table[index][j]!=TSP::M)
        {
            if(min_val == TSP::UNDEFINED || table[index][j]<min_val)
            {
                min_val = table[index][j];
            }
        }
    }

    for(int j=0; j<pointsNumber; j++)
    {
        if(table[index][j]!=TSP::M)
        {
            table[index][j] -= min_val;
        }
    }
}

void OTSP_Solver::reduce_col_vals(TSP_Table &table, int index)
{
    double min_val = TSP::UNDEFINED;
    for(int i=0; i<pointsNumber; i++)
    {
        if(table[i][index]!=TSP::M)
        {
            if(min_val==TSP::UNDEFINED || table[i][index]<min_val)
                min_val = table[i][index];
        }
    }

    for(int i=0; i<pointsNumber; i++)
    {
        if(table[i][index]!=TSP::M)
        {
            table[i][index] -= min_val;
        }
    }
}


void OTSP_Solver::calculate_path()
{
    if(!data)
    {
        throw TSP_Exception("No data!");
    }

    TSP_Table work_table = reduce_table_vals(table);
    while(pointsNumber>1)
    {
        int i,j;
        std::tie(i, j) = getTopRatedPoint(work_table);
        if(i==TSP::UNDEFINED || j==TSP::UNDEFINED)
            throw TSP_Exception("No path.");
        add_to_path(i, j);
        work_table[j][i] = TSP::M;
        reduce_table(work_table, i, j); //pointsNumber--; here

        if(pointsNumber>1)
            work_table = reduce_table_vals(work_table);
    }

    add_to_path(0,0);
}

std::pair<int, int> OTSP_Solver::getTopRatedPoint(TSP_Table &table)
{
    double max_rating = 0.0;
    int r_i = TSP::UNDEFINED, r_j = TSP::UNDEFINED;

    for(int i=0; i<pointsNumber; i++)
        for(int j=0; j<pointsNumber; j++)
        {
            if(table[i][j] == 0)
            {
                double curr_rating = getPointRating(table, i, j);
                if(curr_rating > max_rating)
                {
                    max_rating = curr_rating;
                    r_i = i;
                    r_j = j;
                }else if(curr_rating == max_rating && !path.empty())
                {
                    if(rows[i] == path.back())
                    {
                        max_rating = curr_rating;
                        r_i = i;
                        r_j = j;
                    }
                }


                if(r_i==TSP::UNDEFINED)
                {
                    r_i = i;
                    r_j = j;
                }
            }
        }

    std::pair<int, int> res;
    res.first = r_i;
    res.second = r_j;
    return res;
}

double OTSP_Solver::getPointRating(TSP_Table &table, int r, int c)
{
    double col_min = TSP::UNDEFINED, row_min = TSP::UNDEFINED;

    for(int i=0; i<pointsNumber; i++)
    {
        if(i!=r && table[i][c]!=TSP::M)
        {
            if(col_min==TSP::UNDEFINED || table[i][c]<col_min)
            {
                col_min = table[i][c];
            }
        }

        if(i!=c && table[r][i]!=TSP::M)
        {
            if(row_min==TSP::UNDEFINED || table[r][i]<row_min)
            {
                row_min = table[r][i];
            }
        }
    }

    if(row_min == TSP::UNDEFINED) row_min = 0;
    if(col_min == TSP::UNDEFINED) col_min = 0;

    return (row_min + col_min);
}

void OTSP_Solver::add_to_path(int i, int j)
{
    int abs_i = rows[i];
    int abs_j = cols[j];

    //la first point costille
    if(path.empty())
    {
        path.push_back(abs_i);
    }
    path.push_back(abs_j);

    pathlen += table[abs_i][abs_j];
}

void OTSP_Solver::reduce_table(TSP_Table &table, int i, int j)
{
    table.erase(table.begin()+i);
    for(int t=0; t<(int)table.size(); t++)
    {
        table[t].erase(table[t].begin()+j);
    }

    rows.erase(rows.begin() + i);
    cols.erase(cols.begin() + j);

    pointsNumber--;
}
