"Resource/UI/nb_mod_level_building.res"
{
	"LevelBuildingPanel"
	{
		"fieldName"		"Level_Building_Panel"
		"ControlName"		"LevelBuildingPanel"
		"xpos"		"0"
		"ypos"		"0"
		"wide"		"f0"
		"tall"		"f0"
		"zpos"		"50"
	}
	"Title"
	{
		"fieldName"		"Title"
		"xpos"		"c-210"
		"ypos"		"105"
		"wide"		"250"
		"tall"		"19"
		"font"		"DefaultExtraLarge"
		"textAlignment"		"west"
		"ControlName"		"Label"
		"labelText"		"#asw_building_level_mission_success"
		"fgcolor_override"		"224 224 224 255"
	}
	"Banner"
	{
		"fieldName"		"Banner"
		"xpos"		"0"
		"ypos"		"125"
		"wide"		"f0"
		"tall"		"190"
		"ControlName"		"Panel"		
	}
	"WorkingAnim"
	{
		"ControlName"			"ImagePanel"
		"fieldName"				"WorkingAnim"
		"xpos"					"c-20"
		"ypos"					"190"
		"zpos"					"5"
		"wide"					"40"
		"tall"					"40"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"scaleImage"			"1"
		"image"					"common/swarm_cycle"
		"frame"					"0"
	}		
	"PercentCompleteLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"PercentCompleteLabel"
		"xpos"			"c-100" //original c-175
		"ypos"			"135"
		"zpos"			"0"
		"wide"			"350"
		"tall"			"150"
		"wrap"			"1"
		"font"			"DefaultMedium"
		"textAlignment"   	"west"
		"fgcolor_override"		"169 213 255 255"
	}
	"StatusLabel"
	{
		"ControlName"		"Label"
		"fieldName"		"StatusLabel"
		"xpos"			"c75"
		"ypos"			"135"
		"zpos"			"0"
		"wide"			"350"
		"tall"			"150"
		"wrap"			"1"
		"font"			"DefaultMedium"
		"textAlignment"   	"east"
		"fgcolor_override"		"169 213 255 255"
	}
}
