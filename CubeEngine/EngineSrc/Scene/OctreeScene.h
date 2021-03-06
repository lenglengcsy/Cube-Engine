#ifndef TZW_OCTREESCENE_H
#define TZW_OCTREESCENE_H

#include "../Math/AABB.h"
#include "../Math/Ray.h"
#include <unordered_set>
namespace tzw {
class Drawable3D;
class Camera;
struct OctreeNode
{
    OctreeNode();
    AABB aabb;
    std::vector<Drawable3D *> m_drawlist;
    OctreeNode * m_child[8];
};

class OctreeScene
{
public:
    OctreeScene();
    void init(AABB range);
    void addObj(Drawable3D * obj);
    void removeObj(Drawable3D * obj);
    void updateObj(Drawable3D * obj);
    bool hitByRay(const Ray &ray, vec3 &hitPoint);
    void cullingByCamera(Camera * camera);
    void getRange(std::vector<Drawable3D *> * list,AABB aabb);
    int getAmount();
    std::vector<Drawable3D *>& getVisibleList();
	bool isInOctree(Drawable3D * obj);

private:
    int getAmount_R(OctreeNode * node);
    void cullingByCamera_R(OctreeNode * node,Camera * camera);
    void setIndrawable_R(OctreeNode * node);
    void setDrawable_R(OctreeNode * node);
    void getRange_R(OctreeNode * node, std::vector<Drawable3D *> * list, AABB &aabb);
    OctreeNode * m_root;
    bool addObj_R(OctreeNode * node,Drawable3D * obj);
    bool removeObj_R(OctreeNode * node,Drawable3D * obj);
    bool hitByRay_R(OctreeNode * node,const Ray &ray, vec3 & hitPoint);
    void subDivide(OctreeNode * node,int level);
    std::vector<Drawable3D *> m_visibleList;
	std::unordered_set<Drawable3D *> m_objList;
};

} // namespace tzw

#endif // TZW_OCTREESCENE_H
