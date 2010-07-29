<?php

/* Unofficial Scribd PHP Class library */
//Downloaded from: http://www.scribd.com/platform/resources/libraries
//With edits to request handling (now done by ipaper_request()), variable definitions, and support for xml parsing under php4. also added function getDownloadURL.


class Scribd {

	var $api_key;
	var $secret;
	var $url;
	var $session_key;
	var $my_user_id;
	var $error;
	var $errormsg;
	var $docresult;

	function __construct($api_key, $secret) {
		$this->api_key = $api_key;
		$this->secret = $secret;
		$this->url = "http://api.scribd.com/api?";
	 }


  /**
   * Upload a document from a file
   * @param string $file : relative path to file
   * @param string $doc_type : PDF, DOC, TXT, PPT, etc.
   * @param string $access : public or private. Default is Public.
   * @param int $rev_id : id of file to modify
   * @return array containing doc_id, access_key, and secret_password if nessesary.
   */
	function upload($file, $doc_type = null, $access = null, $rev_id = null){
		$method = "docs.upload";
		$params['doc_type'] = $doc_type;
		$params['access'] = $access;
		$params['rev_id'] = $rev_id;
		$params['file'] = "@".$file;

		$result = $this->postRequest($method, $params);
		return $result;
	}


  /**
   * Upload a document from a Url
   * @param string $url : absolute URL of file 
   * @param string $doc_type : PDF, DOC, TXT, PPT, etc.
   * @param string $access : public or private. Default is Public.
   * @return array containing doc_id, access_key, and secret_password if nessesary.
   */
	function uploadFromUrl($url, $doc_type = null, $access = null, $rev_id = null){
		$method = "docs.uploadFromUrl";
		$params['url'] = $url;
		$params['access'] = $access;
		$params['rev_id'] = $rev_id;
		$params['doc_type'] = $doc_type;

		$data_array = $this->postRequest($method, $params);
		return $data_array;
	}
  /**
   * Get a list of the current users files
   * @return array containing doc_id, title, description, access_key, and conversion_status for all documents
   */
	function getList(){
		$method = "docs.getList";

		$result = $this->postRequest($method, $params);
		return $result['resultset'];
	}
  /**
   * Get the current conversion status of a document
   * @param int $doc_id : document id
   * @return string containing DISPLAYABLE", "DONE", "ERROR", or "PROCESSING" for the current document.
   */
	function getConversionStatus($doc_id){
		$method = "docs.getConversionStatus";
		$params['doc_id'] = $doc_id;

		$result = $this->postRequest($method, $params);
		return $result['conversion_status'];
	}
	/**
	* Get settings of a document
	* @return array containing doc_id, title , description , access, tags, show_ads, license, access_key, secret_password
	*/
	function getSettings($doc_id){
		$method = "docs.getSettings";
		$params['doc_id'] = $doc_id;

		$result = $this->postRequest($method, $params);
		return $result;
	}
  /**
   * Change settings of a document
   * @param array $doc_ids : document id
   * @param string $title : title of document
   * @param string $description : description of document
   * @param string $access : private, or public
   * @param string $license : "by", "by-nc", "by-nc-nd", "by-nc-sa", "by-nd", "by-sa", "c" or "pd"
   * @param string $access : private, or public
   * @param array $show_ads : default, true, or false
   * @param array $tags : list of tags
   * @return string containing DISPLAYABLE", "DONE", "ERROR", or "PROCESSING" for the current document.
   */
	function changeSettings($doc_ids, $title = null, $description = null, $access = null, $license = null, $parental_advisory = null, $show_ads = null, $tags = null, $link_back_url = null){
		$method = "docs.changeSettings";
		$params['doc_ids'] = $doc_ids;
		$params['title'] = $title;
		$params['description'] = $description;
		$params['access'] = $access;
		$params['license'] = $license;
		$params['show_ads'] = $show_ads;
		$params['tags'] = $tags;
		$params['link_back_url'] = $link_back_url;

		$result = $this->postRequest($method, $params);
		return $result;
	}
  /**
   * Delete a document
   * @param int $doc_id : document id
   * @return 1 on success;
   */
	function delete($doc_id){
		$method = "docs.delete";
		$params['doc_id'] = $doc_id;

		$result = $this->postRequest($method, $params);
		return $result;
	}
   /**
   * Search the Scribd database
   * @param string $query : search query
   * @param int $num_results : number of results to return (10 default, 1000 max)
   * @param int $num_start : number to start from
   * @param string $scope : scope of search, "all" or "user"
   * @return array of results, each of which contain doc_id, secret password, access_key, title, and description
   */
	function search($query, $num_results = null, $num_start = null, $scope = null){
		$method = "docs.search";
		$params['query'] = $query;
		$params['num_results'] = $num_results;
		$params['num_start'] = $num_start;
		$params['scope'] = $scope;

		$result = $this->postRequest($method, $params);

		return $result['result_set'];
	}
   /**
   * Get the download URL
   * @param int $doc_id
   * @param string $doc_type: 'pdf', 'txt' or 'original'
   * @return URL as string
   */
	function getDownloadURL($doc_id, $doc_type){

		$method = "docs.getDownloadURL";
		$params['doc_id'] = $doc_id;
		$params['doc_type'] = $doc_type;

		$result = $this->postRequest($method, $params);

		return $result['download_link'];

	}

  /**
   * Login as a user
   * @param string $username : username of user to log in
   * @param string $password : password of user to log in
   * @return array containing session_key, name, username, and user_id of the user;
   */
	function login($username, $password){
		$method = "user.login";
		$params['username'] = $username;
		$params['password'] = $password;

		$result = $this->postRequest($method, $params);
		$this->session_key = $result['session_key'];
		return $result;
	}
  /**
   * Sign up a new user
   * @param string $username : username of user to create
   * @param string $password : password of user to create
   * @param string $email : email address of user
   * @param string $name : name of user
   * @return array containing session_key, name, username, and user_id of the user;
   */
	function signup($username, $password, $email, $name = null){
		$method = "user.signup";
		$params['username'] = $username;
		$params['password'] = $password;
		$params['name'] = $name;
		$params['email'] = $email;

		$result = $this->postRequest($method, $params);
		return $result;
	}
	function postRequest($method, $params){
		$params['api_key'] = $this->api_key;
		$params['method'] = $method;
		$params['session_key'] = $this->session_key;
		$params['my_user_id'] = $this->my_user_id;

		//add
		//query should be accepted as empty for docs.search
		foreach ($params as $key => $val){
			if ($val == null && $key!='query')
				unset($params[$key]);
		}	

		$post_params = array();
		foreach ($params as $key => $val) {
			if (!empty($val) || $key=='query'){
				
				if (is_array($val)) $val = implode(',', $val);
				if($key != 'file' && substr($val, 0, 1) == "@"){
					$val = chr(32).$val;
				}
					
				$post_params[$key] = $val;
			}
		}    
		$secret = $this->secret;
		//add
		if ($secret){
		  $post_params['api_sig'] = $this->generate_sig($params, $secret);
		}
		$request_url = $this->url;

		//moved to function that supports both curl and fopen
		$xml = _ipaper_request($request_url, $post_params, "POST");
		if (function_exists("simplexml_load_string")){
			$result = simplexml_load_string($xml);
		} else {
			$result = $this->ipaper_parse_xml($xml);
		}
 
			if($result['stat'] == 'fail'){
				//This is ineffecient.
				$error_array = (array)$result;
				$error_array = (array)$error_array;
				$error_array = (array)$error_array['error'];
				$error_array = $error_array['@attributes'];
				$this->error = $error_array['code'];
				$this->errormsg = $error_array['message'];

				//throw new Exception($error_array['message'], $error_array['code']);

				return 0;
			
			}
			if($result['stat'] == "ok"){
				
				$this->docresult = $result;
				//This is shifty. Works currently though.
				$result = $this->convert_simplexml_to_array($result);
				if(urlencode((string)$result) == "%0A%0A" && $this->error == 0){
					$result = "1";
					return $result;
				}else{
					return $result;
				}
			}
	}

	function generate_sig($params_array, $secret) {
		$str = '';

		ksort($params_array);
		// Note: make sure that the signature parameter is not already included in
		//       $params_array.
		foreach ($params_array as $k=>$v) {
		  if ($k != 'file')
		  $str .= $k . $v;
		}
		$str = $secret . $str;

		return md5($str);
	}

	function convert_simplexml_to_array($sxml) {
		$arr = array();
		if ($sxml) {
		  foreach ($sxml as $k => $v) {
				if($arr[$k]){
					$arr[$k." ".(count($arr) + 1)] = self::convert_simplexml_to_array($v);
				}else{
					$arr[$k] = self::convert_simplexml_to_array($v);
				}
			}
		}
		if (sizeof($arr) > 0) {
		  return $arr;
		} else {
		  return (string)$sxml;
		}
	}

	function ipaper_parse_xml($xml){
		//drupal_set_message("The SimpleXML library is not available (most likely because you are using php4). The current version of the iPaper module depends on simplexml_load_string(). For more information, see http://drupal.org/node/272299.", 'error');
		return;

		$encoding = 'UTF-8';
		$values = array();
		$index = array();
		$parser = xml_parser_create($encoding);
		xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
		xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);
		$ok = xml_parse_into_struct($parser, $xml, $values, $index);

		if (!$ok) {
			watchdog("ipaper", "Error parsing XML");
		}

		xml_parser_free($parser);
		$result = array();
		foreach($values as $key => $node){
			$result[$node['tag']] = $node['value'];
		}

		$result = $values;
		return $result;
	}

	function getdocresult(){
		return $this->docresult;
	}
	function geterror(){
		return $this->error;
	}
	function geterrormsg(){
		return $this->errormsg;
	}
}


?>