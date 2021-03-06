#pragma once

#include "JKCommon/JKCommon.h"
#include <map>
#include <sstream>
#include <iostream> 
#include <algorithm>
#include "JKStringUtil.h"

BEGIN_JK_NAMESPACE


/** url编码 解码 */
void url_parse(const std::string & url, std::map<std::string, std::string>& params)
{
	int pos = (int)url.find("?");
	if (pos != -1)
	{
		int key_start = pos + 1,
			key_len = 0,
			val_start = 0;
		for (int i = key_start; i <= (int)url.size(); ++i)
		{
			switch (url[i])
			{
			case '=':
				key_len = i - key_start;
				val_start = i + 1;
				break;
			case '\0':
			case '&':
				if (key_len != 0)
				{
					params[url.substr(key_start, key_len)] = url.substr(val_start, i - val_start);
					key_start = i + 1;
					key_len = 0;
				}
				break;
			default:
				break;
			}
		}
	}
}

/** url编码 解码 */
std::string url_encode(const std::string & input, bool encode_slash = true)
{
	std::stringstream ss;
	const char *str = input.c_str();

	for (uint32_t i = 0; i < input.size(); i++)
	{
		unsigned char c = str[i];
		if (isalnum(c) || c == '_' || c == '-' || c == '~' || c == '.' || (!encode_slash && c == '/'))
		{
			ss << c;
		}
		else
		{
			ss << "%" << JKStringUtil::to_hex(c);
		}
	}

	return ss.str();
}


/** 规整化参数 */
std::string canonicalize_params(std::map<std::string, std::string> & params)
{
	std::vector<std::string> v;
	v.reserve(params.size());

	for (auto & it : params) {
		v.push_back(url_encode(it.first) + "=" + url_encode(it.second));
	}
	std::sort(v.begin(), v.end());

	std::string result;
	for (auto & it : v)
	{
		result.append((result.empty() ? "" : "&") + it);
	}
	return result;
}

/** 规整化头 */
std::string canonicalize_headers(std::map<std::string, std::string> & headers)
{
	std::vector<std::string> v;
	v.reserve(headers.size());

	for (auto & it : headers) {
		v.push_back(url_encode(JKStringUtil::to_lower(it.first)) + ":" + url_encode(it.second));
	}
	std::sort(v.begin(), v.end());

	std::string result;
	for (auto & it : v)
	{
		result.append((result.empty() ? "" : "\n") + it);
	}
	return result;
}

std::string get_headers_keys(std::map<std::string, std::string> & headers)
{
	std::vector<std::string> v;
	v.reserve(headers.size());

	for (auto & it : headers) {
		v.push_back(JKStringUtil::to_lower(it.first));
	}

	std::string result;
	for (auto & it : v)
	{
		result.append((result.empty() ? "" : ";") + it);
	}
	return result;
}

std::string get_host(const std::string & url)
{
	int pos = (int)url.find("://") + 3;
	return url.substr(
		pos,
		url.find('/', pos) - pos
	);
}

std::string get_path(const std::string & url)
{
	int path_start = (int)url.find('/', url.find("://") + 3);
	int path_end = (int)url.find('?');
	path_end = path_end == -1 ? (int)url.size() : path_end;

	return url.substr(path_start, path_end - path_start);
}


// #include <openssl/evp.h>
// #include <openssl/hmac.h>
// std::string hmac_sha256(
// 	const std::string & src,
// 	const std::string & sk)
// {
// 	const EVP_MD *evp_md = EVP_sha256();
// 	unsigned char md[EVP_MAX_MD_SIZE];
// 	unsigned int md_len = 0;
// 
// 	if (HMAC(evp_md,
// 		reinterpret_cast<const unsigned char *>(sk.data()), (int)sk.size(),
// 		reinterpret_cast<const unsigned char *>(src.data()), src.size(),
// 		md, &md_len) == NULL)
// 	{
// 		return "";
// 	}
// 
// 	std::stringstream ss;
// 	for (int i = 0; i < (int)md_len; ++i)
// 	{
// 		ss << to_hex(md[i], true);
// 	}
// 
// 	return ss.str();
// }
// 
// void sign(
// 	std::string method,
// 	std::string & url,
// 	std::map<std::string, std::string> & params,
// 	std::map<std::string, std::string> & headers,
// 	std::string & ak,
// 	std::string & sk)
// {
// 	url_parse(url, params);
// 	headers["Host"] = get_host(url);
// 	std::string timestamp = utc_time(now());
// 	headers["x-bce-date"] = timestamp;
// 
// 	std::stringstream ss;
// 	ss << "bce-auth-v" << __BCE_VERSION__ << "/" << ak << "/"
// 		<< timestamp << "/" << __BCE_EXPIRE__;
// 
// 	std::string val = ss.str();
// 	std::string sign_key = hmac_sha256(val, sk);
// 
// 	ss.str("");
// 	ss << to_upper(method) << '\n' << url_encode(get_path(url), false)
// 		<< '\n' << canonicalize_params(params)
// 		<< '\n' << canonicalize_headers(headers);
// 
// 	std::string signature = hmac_sha256(ss.str(), sign_key);
// 
// 	ss.str("");
// 	ss << "bce-auth-v" << __BCE_VERSION__ << "/" << ak << "/"
// 		<< timestamp << "/" << __BCE_EXPIRE__ << "/"
// 		<< get_headers_keys(headers) << "/" << signature;
// 
// 	headers["authorization"] = ss.str();
// }


// void makeUrlencodedForm(std::map<std::string, std::string> const & params, std::string * content) const
// {
// 	content->clear();
// 	std::map<std::string, std::string>::const_iterator it;
// 	for (it = params.begin(); it != params.end(); it++)
// 	{
// 		char * key = curl_escape(it->first.c_str(), (int)it->first.size());
// 		char * value = curl_escape(it->second.c_str(), (int)it->second.size());
// 		*content += key;
// 		*content += '=';
// 		*content += value;
// 		*content += '&';
// 		curl_free(key);
// 		curl_free(value);
// 	}
// }
// 
// void appendUrlParams(std::map<std::string, std::string> const & params, std::string* url) const
// {
// 	if (params.empty()) {
// 		return;
// 	}
// 	std::string content;
// 	this->makeUrlencodedForm(params, &content);
// 	bool url_has_param = false;
// 	for (const auto& ch : *url) {
// 		if (ch == '?') {
// 			url_has_param = true;
// 			break;
// 		}
// 	}
// 	if (url_has_param) {
// 		url->append("&");
// 	}
// 	else {
// 		url->append("?");
// 	}
// 	url->append(content);
// }
// 
// void appendHeaders(std::map<std::string, std::string> const & headers, curl_slist ** slist) const
// {
// 	std::ostringstream ostr;
// 	std::map<std::string, std::string>::const_iterator it;
// 	for (it = headers.begin(); it != headers.end(); it++)
// 	{
// 		ostr << it->first << ":" << it->second;
// 		*slist = curl_slist_append(*slist, ostr.str().c_str());
// 		ostr.str("");
// 	}
// }


END_JK_NAMESPACE
