#include "KSLabelTTF.h"

void KSLabelTTF::enableOuterStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture)
{
	m_outerIsStroke = true;
	m_outerStrokeColor = strokeColor;
	m_outerStrokeSize = strokeSize;
	setString(m_string.c_str());
}

void KSLabelTTF::disableOuterStroke(bool mustUpdateTexture)
{
	m_outerIsStroke = false;
}
void KSLabelTTF::setString(const char *labelStr)
{
	CCLabelTTF::setString(labelStr);
	
	CCLabelTTF* label = this;
	auto oFlip = label->isFlipY();
	auto oColor = label->getColor();
	auto oPosition = label->getPosition();
	if(m_outerIsStroke)
	{
		CCRenderTexture* rt = CCRenderTexture::create(label->getTexture()->getContentSize().width + m_outerStrokeSize*2 , label->getTexture()->getContentSize().height+m_outerStrokeSize*2);

		label->setFlipY(true);
		label->setColor(m_outerStrokeColor);

		ccBlendFunc originalBlendFunc = label->getBlendFunc();
		ccBlendFunc _t = {GL_SRC_ALPHA, GL_ONE};
		label->setBlendFunc(_t);
		CCPoint bottomLeft = ccp(label->getTexture()->getContentSize().width * label->getAnchorPoint().x + m_outerStrokeSize, label->getTexture()->getContentSize().height * label->getAnchorPoint().y + m_outerStrokeSize);
		CCPoint position = ccpSub(label->getPosition(), ccp(-label->getContentSize().width / 2.0f,-label->getContentSize().height / 2.0f));

		rt->begin();

			for (int i=0; i<360; i++) // you should optimize that for your needs
			{
				label->setPosition(ccp(bottomLeft.x + sin(CC_DEGREES_TO_RADIANS(i))*m_outerStrokeSize,bottomLeft.y + cos(CC_DEGREES_TO_RADIANS(i))*m_outerStrokeSize));
				label->visit();
			}

			label->setPosition(bottomLeft);
			label->setBlendFunc(originalBlendFunc);
			label->setColor(m_textFillColor);
			label->visit();

		rt->end();
		label->setFlipY(oFlip);
		//if(m_outerSprite)
			//m_outerSprite->removeFromParent();
		//m_outerSprite = CCSprite::createWithTexture(rt->getSprite()->getTexture());
		//addChild(m_outerSprite);
		//m_outerSprite->setPosition(bottomLeft + ccp(0, 0));
		CCLabelTTF::setTextureRect(rt->getSprite()->getTextureRect());
		CCLabelTTF::setTexture(rt->getSprite()->getTexture());
	}
	
}
