foo = newScript()
obj = $Label
in coordSys obj (
	format "// % -------------------------------------------------------\n" obj.name to:foo
	format "mesh.setNumVerts(%);\n" obj.numVerts to:foo
	format "mesh.setNumFaces(%);\n" obj.numFaces to:foo
	for i in 1 to obj.numverts do (
		local v = getVert obj i
		v = Bfd.Math.Round v 1
		setVert obj i v.x v.y v.z
		format "mesh.setVert(%,scale*Point3(%f,%f,%f));\n" (i-1) v.x v.y v.z to:foo
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
