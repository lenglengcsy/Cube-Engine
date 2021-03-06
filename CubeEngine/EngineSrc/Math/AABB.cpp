#include "AABB.h"
#include "math.h"
#include <algorithm>

namespace tzw {

AABB::AABB()
{
    reset();
}

AABB::~AABB()
{

}

void AABB::update(vec3 *vec, int num)
{
    for (size_t i = 0; i < num; i++)
    {
        // Leftmost point.
        if (vec[i].x < m_min.x)
            m_min.setX(vec[i].x);

        // Lowest point.
        if (vec[i].y < m_min.y)
            m_min.setY (vec[i].y);

        // Farthest point.
        if (vec[i].z < m_min.z)
            m_min.setZ (vec[i].z);

        // Rightmost point.
        if (vec[i].x > m_max.x)
            m_max.setX(vec[i].x);


        // Highest point.
        if (vec[i].y > m_max.y)
            m_max.setY (vec[i].y);

        // Nearest point.
        if (vec[i].z > m_max.z)
            m_max.setZ (vec[i].z);
    }
}

void AABB::update(vec3 vec)
{
    // Leftmost point.
    if (vec.x < m_min.x)
        m_min.setX(vec.x);

    // Lowest point.
    if (vec.y < m_min.y)
        m_min.setY (vec.y);

    // Farthest point.
    if (vec.z < m_min.z)
        m_min.setZ (vec.z );

    // Rightmost point.
    if (vec.x > m_max.x)
        m_max.setX(vec.x );


    // Highest point.
    if (vec.y > m_max.y)
        m_max.setY (vec.y );

    // Nearest point.
    if (vec.z > m_max.z)
        m_max.setZ (vec.z );
}

void AABB::transForm(Matrix44 mat)
{

    vec3 corners[8];
     // Near face, specified counter-clockwise
    // Left-top-front.
    corners[0] = vec3(m_min.x, m_max.y, m_max.z);
    // Left-bottom-front.
    corners[1] = vec3(m_min.x, m_min.y, m_max.z);
    // Right-bottom-front.
    corners[2] = vec3(m_max.x, m_min.y, m_max.z);
    // Right-top-front.
    corners[3] = vec3(m_max.x, m_max.y, m_max.z);

    // Far face, specified clockwise
    // Right-top-back.
    corners[4] = vec3(m_max.x, m_max.y, m_min.z);
    // Right-bottom-back.
    corners[5] = vec3(m_max.x, m_min.y, m_min.z);
    // Left-bottom-back.
    corners[6] = vec3(m_min.x, m_min.y, m_min.z);
    // Left-top-back.
    corners[7] = vec3(m_min.x, m_max.y, m_min.z);
    for(int i =0 ; i < 8 ; i++)
    {
        vec4 result = mat*vec4(corners[i].x,corners[i].y,corners[i].z,1);
        corners[i] = vec3(result.x,result.y,result.z);
    }
    reset();
    update(corners,8);
}

void AABB::reset()
{
    m_min = vec3(999,999,999);
    m_max = vec3(-999,-999,-999);
}

void AABB::merge(AABB box)
{
        // Calculate the new minimum point.
        m_min.setX (std::min(m_min.x, box.min ().x));
        m_min.setY (std::min(m_min.y, box.min ().y));
        m_min.setZ (std::min(m_min.z, box.min ().z));

        // Calculate the new maximum point.
        m_max.setX (std::max(m_max.x, box.max().x));
        m_max.setY(std::max(m_max.y, box.max().y));
        m_max.setZ(std::max(m_max.z, box.max().z));
}

vec3 AABB::min() const
{
    return m_min;
}

void AABB::setMin(const vec3 &min)
{
    m_min = min;
}
vec3 AABB::max() const
{
    return m_max;
}

vec3 AABB::centre()
{
    return (m_max + m_min)/2;
}

/**
 * @brief 将当前AABB八等分，返回八个子AABB
 * @note 可以用来构建八叉树
 * @return 八个子AABB
 */
std::vector<AABB> AABB::split8()
{
    AABB aabb_list[8];
    std::vector<AABB> result;
    auto the_centre = centre();

    //bottom
    //#1
    aabb_list[0].update(m_min);
    aabb_list[0].update(the_centre);
    //#2
    aabb_list[1].update(vec3(the_centre.x,m_min.y,m_min.z));
    aabb_list[1].update(vec3(m_max.x,the_centre.y,the_centre.z));
    //#3
    aabb_list[2].update(vec3(the_centre.x,m_min.y,the_centre.z));
    aabb_list[2].update(vec3(m_max.x,the_centre.y,m_max.z));
    //#4
    aabb_list[3].update(vec3(m_min.x,m_min.y,the_centre.z));
    aabb_list[3].update(vec3(the_centre.x,the_centre.y,m_max.z));

    //top
    //#5
    aabb_list[4].update(vec3(m_min.x,the_centre.y,m_min.z));
    aabb_list[4].update(vec3(the_centre.x,m_max.y,the_centre.z));
    //#6
    aabb_list[5].update(vec3(the_centre.x,the_centre.y,m_min.z));
    aabb_list[5].update(vec3(m_max.x,m_max.y,the_centre.z));
    //#7
    aabb_list[6].update(vec3(the_centre.x,the_centre.y,the_centre.z));
    aabb_list[6].update(vec3(m_max.x,m_max.y,m_max.z));
    //#8
    aabb_list[7].update(vec3(m_min.x,the_centre.y,the_centre.z));
    aabb_list[7].update(vec3(the_centre.x,m_max.y,m_max.z));

    for(int i =0;i<8;i++)
    {
        result.push_back(aabb_list[i]);
    }
    return result;
}

bool AABB::isInside(vec3 p) const
{
    if(p.x>=m_min.x&&p.y>=m_min.y && p.z>= m_min.z
            &&p.x<=m_max.x && p.y <=m_max.y && p.z <= m_max.z)
    {
        return true;
    }
    else{
        return false;
    }
}

void AABB::setMax(const vec3 &max)
{
    m_max = max;
}

bool AABB::isCanCotain(AABB aabb)
{
    if(m_min.x <aabb.min ().x && m_min.y <aabb.min ().y && m_min.z <aabb.min ().z
            && m_max.x>aabb.max ().x && m_max.y>aabb.max ().y && m_max.z>aabb.max ().z)
    {
        return true;
    }else
    {
        return false;
    }
}

bool AABB::isIntersect(AABB aabb,vec3 &overLap)
{
    bool isHit= ((m_min.x >= aabb.m_min.x && m_min.x <= aabb.m_max.x) || (aabb.m_min.x >= m_min.x && aabb.m_min.x <= m_max.x)) &&
           ((m_min.y >= aabb.m_min.y && m_min.y <= aabb.m_max.y) || (aabb.m_min.y >= m_min.y && aabb.m_min.y <= m_max.y)) &&
           ((m_min.z >= aabb.m_min.z && m_min.z <= aabb.m_max.z) || (aabb.m_min.z >= m_min.z && aabb.m_min.z <= m_max.z));
    if(isHit)
    {
        if(min().x<aabb.min().x || max().x >aabb.max().x)
        {
            float offset_x_1 =aabb.max().x - min().x,offset_x_2 = aabb.min().x - max().x;
            if(fabs(offset_x_1)< fabs(offset_x_2))
            {
                overLap.x = offset_x_1;
            }else{
                overLap.x = offset_x_2;
            }
        }
        if(min().y<aabb.min().y || max().y >aabb.max().y)
        {
            float offset_y_1 =aabb.max().y - min().y,offset_y_2 = aabb.min().y - max().y;

            if(fabs(offset_y_1)< fabs(offset_y_2))
            {
                overLap.y = offset_y_1;
            }else{
                overLap.y = offset_y_2;
            }

        }
        if(min().z<aabb.min().z || max().z >aabb.max().z)
        {
            float offset_z_1 =aabb.max().z - min().z,offset_z_2 = aabb.min().z - max().z;

            if(fabs(offset_z_1)< fabs(offset_z_2))
            {
                overLap.z = offset_z_1;
            }else{
                overLap.z = offset_z_2;
            }
        }

    }
    return isHit;
}
} // namespace tzw










