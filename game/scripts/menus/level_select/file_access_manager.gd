extends Node

@onready var fd_upload = $FileDialogUpload

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	fd_upload.file_mode = FileDialog.FILE_MODE_OPEN_FILES
	fd_upload.current_dir = "/"


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_file_dialog_upload_file_selected(path: String) -> void:
	pass # Replace with function body.
