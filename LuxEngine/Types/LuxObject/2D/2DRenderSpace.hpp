#pragma once
#define LUX_H_RENDER_SPACE_2D
#include "LuxEngine/Types/LuxObject/LuxObject.hpp"
#include "LuxEngine/Types/LuxObject/2D/Obj2_b.hpp"




namespace lux::obj {
	enum class AlignmentType : int16{
		Horizontal,
		Vertical,
		FixedHorizontal,
		FixedVertical,
		FixedGrid,
		Free
	};

	enum class FlowType : int16{
		LeftToRight,
		RightToLeft,
		TopToBottom,
		BottomToTop
	};


	//TODO add object boundary debug draw


	/**
	 * @brief A 2D space that allows objects to be rendered.
	 *		This render space can only be a child of a window or another RenderSpace2D
	 */
	struct RenderSpace2D : public Obj2_b {
		bool allowOutOfViewRender = false;			//Allow out of limit object regions to be rendered

		AlignmentType alignment;	//Type of children alignmen
		uint32 xNum;				//Number of horizontally aligned children
		uint32 yNum;				//Number of vertically aligned children
		FlowType xFlow;				//Children horizontal direction
		FlowType yFlow;				//Children vertical direction


		RenderSpace2D(AlignmentType vAlignment, uint32 vXNum = 2, uint32 vYNum = 2, FlowType vXFlow = FlowType::LeftToRight, FlowType vYFlow = FlowType::TopToBottom)
			: alignment(vAlignment), xNum(vXNum), yNum(vYNum), xFlow(vXFlow), yFlow(vYFlow) {
			luxInitObject(2, RENDER_SPACE);
		}


		bool addChild(Obj2_b* pObject);
		bool setChildLimits(const uint32 vChildIndex) const final;

		//! 0 BY DEFAULT
		// inline int32 getCellSize() const final { return 0; }
	};
}
