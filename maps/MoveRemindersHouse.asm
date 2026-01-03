	object_const_def
	const MOVEREMINDERSHOUSE_SUPER_NERD

MoveRemindersHouse_MapScripts:
	def_scene_scripts

	def_callbacks

MoveReminder:
	faceplayer
	opentext
	special MoveReminder
	waitbutton
	closetext
	end

MoveRemindersHouseBookshelf:
	jumpstd DifficultBookshelfScript

MoveRemindersHouse_MapEvents:
	db 0, 0 ; filler

	def_warp_events
	warp_event  2,  7, BLACKTHORN_CITY, 6
	warp_event  3,  7, BLACKTHORN_CITY, 6

	def_coord_events

	def_bg_events
	bg_event  0,  1, BGEVENT_READ, MoveRemindersHouseBookshelf
	bg_event  1,  1, BGEVENT_READ, MoveRemindersHouseBookshelf

	def_object_events
	object_event  2,  3, SPRITE_SUPER_NERD, SPRITEMOVEDATA_STANDING_DOWN, 0, 0, -1, -1, 0, OBJECTTYPE_SCRIPT, 0, MoveReminder, -1
