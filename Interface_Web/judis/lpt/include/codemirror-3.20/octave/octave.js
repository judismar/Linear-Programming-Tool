CodeMirror.defineMode("octave", function() {
  function wordRegexp(words) {
    return new RegExp("^((" + words.join(")|(") + "))\\b");
  }

  var operators = new RegExp("^[\\+\\-]");
  var specialOperators = new RegExp("^((<=)|(>=)|(\=))");
  var expressionEnd = new RegExp("\0");
  var identifiers = new RegExp("^[_A-Za-z][_A-Za-z0-9]*");

  var builtins = wordRegexp([
    'free', 'negative', 'positive'
  ]);

  var keywords = wordRegexp([
    'min', 'max', 'subject to', 'variable bounds'
  ]);

  var specialkeywords = /^(s\.t\.)|(v\.b\.)/;

  // tokenizers
  function tokenTranspose(stream, state) {
    if (!stream.sol() && stream.peek() === '\'') {
      stream.next();
      state.tokenize = tokenBase;
      return 'operator';
    }
    state.tokenize = tokenBase;
    return tokenBase(stream, state);
  }

  function tokenBase(stream, state) {
    // whitespaces
    if (stream.eatSpace()) return null;

    // Handle one line Comments
    if (stream.match(/^(%)/)){
      stream.skipToEnd();
      return 'comment';
    }

    // Handle Number Literals
    if (stream.match(/^[0-9\.+-]/, false)) {
      if (stream.match(/^\d+\.\d+/)) { return 'number'; }; //real
      if (stream.match(/^\d+\/\d+/)) { return 'number'; }; //fracionário
      if (stream.match(/^\d+/)) { //inteiro
        stream.tokenize = tokenBase;
        return 'number'; };
    }

    // Handle words
    if (stream.match(keywords)) { return 'keyword'; } ;
    if (stream.match(specialkeywords)) { return 'keyword'; } ;
    if (stream.match(builtins)) { return 'builtin'; } ;
    if (stream.match(identifiers)) { return 'variable'; } ;

    if (stream.match(operators)) { return 'operator'; };
	if (stream.match(specialOperators)) { return 'special-operator'; };

    if (stream.match(expressionEnd)) {
      state.tokenize = tokenTranspose;
      return null;
    };


    // Handle non-detected items
    stream.next();
    return 'error';
  };


  return {
    startState: function() {
      return {
        tokenize: tokenBase
      };
    },

    token: function(stream, state) {
      var style = state.tokenize(stream, state);
      if (style === 'number' || style === 'variable'){
        state.tokenize = tokenTranspose;
      }
      return style;
    }
  };
});

CodeMirror.defineMIME("text/x-octave", "octave");
