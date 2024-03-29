/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Gui.h"

#include <OgreRoot.h>

namespace MyGUI
{

	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	RenderItem::RenderItem(const std::string& _texture, LayerItemKeeper * _parent) :
		mParent(_parent),
		mTextureName(_texture),
		mOutDate(false),
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK)
	{
		mRenderSystem = Ogre::Root::getSingleton().getRenderSystem();
		mTextureManager = Ogre::TextureManager::getSingletonPtr();
		//mMaximumDepth = mRenderSystem->getMaximumDepthInputValue();

		createVertexBuffer();

		// Initialise blending modes to be used. We use these every frame, so we'll set them up now to save time later.
		mColorBlendMode.blendType	= Ogre::LBT_COLOUR;
		mColorBlendMode.source1		= Ogre::LBS_TEXTURE;
		mColorBlendMode.source2		= Ogre::LBS_DIFFUSE;
		mColorBlendMode.operation	= Ogre::LBX_MODULATE;

		mAlphaBlendMode.blendType	= Ogre::LBT_ALPHA;
		mAlphaBlendMode.source1		= Ogre::LBS_TEXTURE;
		mAlphaBlendMode.source2		= Ogre::LBS_DIFFUSE;
		mAlphaBlendMode.operation	= Ogre::LBX_MODULATE;

		mTextureAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;

		mLayerManager = LayerManager::getInstancePtr();

	}

	RenderItem::~RenderItem()
	{
		destroyVertexBuffer();
	}

	void RenderItem::createVertexBuffer()
	{
		mRenderOperation.vertexData = new Ogre::VertexData();
		mRenderOperation.vertexData->vertexStart = 0;

		Ogre::VertexDeclaration* vd = mRenderOperation.vertexData->vertexDeclaration;
		vd->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
		vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ), Ogre::VET_COLOUR, Ogre::VES_DIFFUSE );
		vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ) +
						   Ogre::VertexElement::getTypeSize( Ogre::VET_COLOUR ),
						   Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

		// Create the Vertex Buffer, using the Vertex Structure we previously declared in _declareVertexStructure.
		mVertexBuffer = Ogre::HardwareBufferManager::getSingleton( ).createVertexBuffer(
			mRenderOperation.vertexData->vertexDeclaration->getVertexSize(0), // declared Vertex used
			mVertexCount,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
			false );

		// Bind the created buffer to the renderOperation object.  Now we can manipulate the buffer, and the RenderOp keeps the changes.
		mRenderOperation.vertexData->vertexBufferBinding->setBinding( 0, mVertexBuffer );
		mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOperation.useIndexes = false;
	}

	void RenderItem::destroyVertexBuffer()
	{
		delete mRenderOperation.vertexData;
		mRenderOperation.vertexData = 0;
		mVertexBuffer.setNull();
	}

	void RenderItem::initRenderState()
	{
		// set-up matrices
		mRenderSystem->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setViewMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

		// initialise render settings
		mRenderSystem->setLightingEnabled(false);
		mRenderSystem->_setDepthBufferParams(false, false);
		mRenderSystem->_setDepthBias(0, 0);
		mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
		mRenderSystem->_setFog(Ogre::FOG_NONE);
		mRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		mRenderSystem->setShadingType(Ogre::SO_GOURAUD);
		mRenderSystem->_setPolygonMode(Ogre::PM_SOLID);

		// initialise texture settings
		mRenderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		mRenderSystem->_setTextureCoordSet(0, 0);
		mRenderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
		mRenderSystem->_setTextureAddressingMode(0, mTextureAddressMode);
		mRenderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0,true);
		mRenderSystem->_setTextureBlendMode(0, mColorBlendMode);
		mRenderSystem->_setTextureBlendMode(0, mAlphaBlendMode);
		mRenderSystem->_disableTextureUnitsFrom(1);


		// enable alpha blending
		mRenderSystem->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
	}

	void RenderItem::resizeVertexBuffer()
	{
		mVertexCount = mNeedVertexCount + RENDER_ITEM_STEEP_REALLOCK;
		destroyVertexBuffer();
		createVertexBuffer();
	}

	void RenderItem::_render(bool _update)
	{
		if (mTextureName.empty()) return;
		if (mNeedVertexCount > mVertexCount) resizeVertexBuffer();

		if (mOutDate || _update) {

			Vertex * buffer = (Vertex*)mVertexBuffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD);


			size_t count_all = 0;
			for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter) {
				size_t count = (*iter).first->_drawItem(buffer, _update);
				// ����������� ������������ ������
				MYGUI_DEBUG_ASSERT(count <= (*iter).second, "It is too much vertexes");
				buffer += count;
				count_all += count;
			}

			mRenderOperation.vertexData->vertexCount = count_all;
			mVertexBuffer->unlock();

			mOutDate = false;
		}

		if (false == mTextureManager->resourceExists(mTextureName))
			mTextureManager->load(mTextureName, Gui::getInstance().getResourceGroup());

		// set texture that will be applied to all vertices rendered.
		mRenderSystem->_setTexture(0, true, mTextureName);
		// set render properties prior to rendering.
		initRenderState();
		// perform the rendering.
		mRenderSystem->_render(mRenderOperation);

		mLayerManager->_addBatch();
	}

	/*void RenderItem::_resize(const FloatSize& _size)
	{
		mPixScaleX = 1.0 / _size.width;
		mPixScaleY = 1.0 / _size.height;
		mAspectCoef = _size.height / _size.width;

        mHOffset = mRenderSystem->getHorizontalTexelOffset() / _size.width;
        mVOffset = mRenderSystem->getVerticalTexelOffset() / _size.height;

		mOutDate = true;
	}*/

	void RenderItem::removeDrawItem(DrawItem * _item)
	{
		for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter) {
			if ((*iter).first == _item) {
				mNeedVertexCount -= (*iter).second;
				mDrawItems.erase(iter);
				mOutDate = true;

				// ���� ��� ������������, ��������� ����
				if (mDrawItems.empty()) {
					mTextureName.clear();
					mParent->_update();
				}

				return;
			}
		}
		MYGUI_EXCEPT("DrawItem not found");
	}

} // namespace MyGUI
