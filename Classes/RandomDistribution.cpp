#include "RandomDistribution.h"
#include <vector>
RandomDistribution::RandomDistribution()
{
	
}
RandomDistribution::~RandomDistribution()
{
}

bool RandomDistribution::init()
{
	CCLayer::init();
	
	srand(time(NULL));
	std::random_device rd;
	m_rEngine.seed(rd());
	
	
	CCDrawNode* dn = CCDrawNode::create();
	addChild(dn);

	std::normal_distribution<> distX(240, 60.f);
	// C++ 11
	std::map<int, int> datas;
	for(int loop=0; loop<3000; loop++)
	{
		datas[distX(m_rEngine)]++;
	}
	for(auto d : datas)
	{
		dn->drawSegment(ccp(d.first, 0), ccp(d.first, d.second*10.f), 1, ccc4f(1, 0, 0, 0.5f));
	}
	

	
	
	
	return true;
}


void RandomDistribution::update(float dt)
{
	
}