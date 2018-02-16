var editor = CodeMirror.fromTextArea(document.getElementById("inputprogram"),
{
	mode:
	{
		name: "octave",
		version: 2,
		singleLineStringErrors: false
	},
	lineNumbers: true,
	indentUnit: 4,
	tabMode: "shift",
	matchBrackets: true
});
editor.setSize(730, 240);
