--requires: BfdFunctions.ms

--mesh.setNumVerts(nverts);
--mesh.setNumFaces(nfaces);
--mesh.setVert(0,Point3(v.x,v.y,v.z));
--mesh.faces[0].setVerts(1,2,3);
--mesh.faces[0].setEdgeVisFlags(1,0,1);

objs = #($Cross_Solid, $Cross_Wire,
		$Jack_Solid, $Jack_Wire,
		$Box_Solid, $Box_Wire,
		$Sphere_Solid, $Sphere_Wire,
		$Vector_Solid, $Vector_Wire,
		$Turtle_Solid, $Turtle_Wire,
		$Line_Solid, $Line_Wire)

objsPrec = #(2, 2,
			2, 2,
			1, 1,
			2, 2,
			3, 3,
			2, 2,
			2, 2)


foo = newScript()
for objIdx in 1 to objs.count do (
	local obj = objs[objIdx]
	in coordSys obj (
		format "// % -------------------------------------------------------\n" obj.name to:foo
		format "mesh.setNumVerts(%);\n" obj.numVerts to:foo
		format "mesh.setNumFaces(%);\n" obj.numFaces to:foo
		for i in 1 to obj.numverts do (
			local v = getVert obj i
			v = Bfd.Math.Round v objsPrec[objIdx]
			setVert obj i v.x v.y v.z
			format "mesh.setVert(%,Point3((sx*%f),(sy*%f),(sz*%f)));\n" (i-1) v.x v.y v.z to:foo
		)
		update obj
		for i in 1 to obj.numfaces do (
			local f = getFace obj i
			format "mesh.faces[%].setVerts(%,%,%);\n" (i-1) ((f.x-1) as integer) ((f.y-1) as integer) ((f.z-1) as integer) to:foo
			format "mesh.faces[%].setEdgeVisFlags(%,%,%);\n" (i-1)	(if (getEdgeVis obj i 1) then 1 else 0) \
																	(if (getEdgeVis obj i 2) then 1 else 0) \
																	(if (getEdgeVis obj i 3) then 1 else 0) to:foo
		)
		format "\n" to:foo
	)
)
