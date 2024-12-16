extends Control

@export var tester: GoogleTestWrapper

func _on_exit_button_pressed() -> void:
	self.get_tree().quit(0)
	pass

func _on_run_test_button_pressed() -> void:
	print(OS.get_executable_path())
	print(tester.run_test()) # not working
	pass
