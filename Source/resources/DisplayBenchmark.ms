---------------------------------------------------------------------------------------------------
-- Create a scene to test the display speed of Nulls
---------------------------------------------------------------------------------------------------

(
	-- Resolution of the grid of nulls
	local xRes = 32
	local yRes = 32

	-- #standard, #complex
	local nullType = #standard

---------------------------------------------------------------------------------------------------

	ResetMaxFile #noPrompt

	with redraw off
	undo off
	(
		local sourceNull = case nullType of
		(
			#standard:	Null	lockXYZ:true				\
								xScale:5.0					\
								solid:false					\
								nullShape:2					\
								displayLabel:false			\
								label:""					\
								nonScalingDisplay:false		\
								colorAxis:true				\
								displayParentName:false		\
								displayLinkLine:false		\
								displayName:false			\
								mixWireColor:true			\
								displayCenterPoint:false
			#complex:	Null	lockXYZ:false				\
								xScale:5.0					\
								yScale:4.0					\
								zScale:3.0					\
								solid:false					\
								nullShape:2					\
								displayLabel:true			\
								label:"Test Label"			\
								nonScalingDisplay:false		\
								colorAxis:true				\
								displayParentName:true		\
								displayLinkLine:true		\
								displayName:true			\
								mixWireColor:true			\
								displayCenterPoint:true
			default:	Throw "Unrecognized null type"
		)

		local parentNull = Null		lockXYZ:true				\
									xScale:5.0					\
									solid:true					\
									nullShape:3					\
									displayLabel:false			\
									label:""					\
									nonScalingDisplay:false		\
									colorAxis:false				\
									displayParentName:false		\
									displayLinkLine:false		\
									displayName:false			\
									mixWireColor:false			\
									displayCenterPoint:false	\
									wireColor:black				\
									name:"ParentNull"

		local instanceNulls = #()
		local centerPos = [0,0,0]

		for iy in 1 to yRes do
		(
			for ix in 1 to xRes do
			(
				local instanceNull = Instance sourceNull

				instanceNull.pos = [(ix-1)*10.0, (iy-1)*10.0, 0.0]
				instanceNull.wireColor = yellow
				instanceNull.name = "Null_" + (ix as string) + "_" + (iy as string)

				centerPos += instanceNull.pos
				Append instanceNulls instanceNull
			)
		)
		centerPos = centerPos / instanceNulls.count
		parentNull.pos = centerPos

		for instanceNull in instanceNulls do instanceNull.parent = parentNull

		-- Create an animated camera to force full redraw each frame
		renderWidth = renderHeight = 500
		local cam = FreeCamera	pos:centerPos			\
								type:#free				\
								orthoProjection:true	\
								fov:90					\
								targetDistance:(centerPos.x + (sourceNull.xScale * 0.5))
		animationRange = Interval 0 100
		animate on at time animationRange.end cam.fov = cam.fov

		viewport.SetLayout #layout_1
		viewport.SetCamera cam
		displaySafeFrames = true
		realTimePlayback = false

		Delete sourceNull
	)
)
