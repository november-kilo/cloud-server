/*
 * parse a token-parameter
 */
mixed *tokenparam(string str)
{
    return parse_string("\
ws = /[ \t]+/								\
token = /[-!#$%&'*+.^_`|~A-Za-z0-9]+/					\
string = /\"([^\x00-\x08\x0a-\x1f\x7f\"\\\\]|\\\\[^\x00-\x08\x0a-\x1f\x7f])*\"/										\
junk = /./								\
\
TokenParam: token Params				? token		\
\
Params:									\
Params: Params Param							\
\
Param: Ws ';' Ws token Ws '=' Ws Value			? param		\
\
Ws:									\
Ws: ws							? null		\
\
Value: token								\
Value: string",	str);
}

/*
 * token params
 */
static mixed *token(mixed *parsed)
{
    return ({ parsed[0], parsed[1 ..] });
}

/*
 * param=value
 */
static mixed *param(mixed *parsed)
{
    return ({ parsed[1] + "=" + parsed[3] });
}

/*
 * nothing
 */
static mixed *null(mixed parsed)
{
    return ({ });
}
