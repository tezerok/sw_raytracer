/*

// Fulfills the Scene concept while using spatial partitioning
//  to accelerate intersection finding (unlike BasicScene).
class AcceleratedScene
{
public:
	explicit AcceleratedScene(std::vector<Object> objects);

	Hit findFirstHit(const Ray& ray) const;

	template<typename ObjectType>
	void addObject(ObjectType&& object);

private:
	std::vector<Object> objects;
	Octree<Object> objectTree;
};

*/
