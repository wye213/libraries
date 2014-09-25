<?php

/* Author：Tony yin
 * Date:2014-9-25
 * Description:参考一致性哈希方案实现的一致性哈希库，这种方式效率比较低下，查找时需要遍历数组，函数里的print_r函数用于测试
 */


/* 哈希函数处理接口 便于扩展采用不同的哈希函数实现 目前采用MD5或是crc32实现 */
interface Hash_handler {
	public function hash($str);	
}

class md5_handler implements Hash_handler {
	public function hash($str) {
		return substr(md5($str), 0, 8);
	}
}

class crc32_handler implements Hash_handler {
	public function hash($str) {
		return crc32($str);
	}
}


class Negi_exception extends Exception {

}

class Negi_hash {
	private $virtual_node = 64;
	private $hash_algorithm;
	private $node_count = 0;
	private $position_sorted = False;
	private $node_position = array();  /* 实际节点对应的多个虚拟节点位置  node => [postition, position...] */
	private $position_node = array();  /* 虚拟位置节点对应的实际节点 postion => node */

	public function __construct(Hash_handler $hash = null, $node = null) {
		$this->hash_algorithm = $hash ? $hash : new md5_handler();
		if (!empty($node)) $this->virtual_node = $node;
	}

	/* 添加一个实际节点 同时分成64个虚拟节点 这一步实际就是在将节点服务器映射到hash空间中 */
	public function add_node($node) {
		if (isset($this->node_position[$node])) {
			throw new Negi_exception("The node want to added is exists");
		}

		$this->node->position[$node] = array();

		for ($i = 0; $i < $this->virtual_node; $i++) {
			$position = $this->hash_algorithm->hash($node . $i);
			$this->position_node[$position] = $node;
			$this->node_position[$node][] = $position; 
		}

		$this->node_count++;
		print_r("succesed to add a node " . $node . "\n");
		return $this;
	}

	/* 同时添加多个实际节点 以添加一个实际节点为基础 同上 */
	public function add_nodes($nodes) {
		foreach ($nodes as $node) {
			$this->add_node($node);
		}

		print_r("Succesed to add multi node " . implode(",", $nodes) . " \n");
		return $this;
	}

	/* 删除一个实际节点映射到hash空间上的虚拟节点 */
	public function remove_node($node) {
		if (!isset($this->node_position[$node])) {
			throw new Negi_exception("The node want to removes is not exists");
		}

		foreach ($this->node_position[$node] as $position) {
			unset($this->position_node[$position]);
		}

		unset($this->node_position[$node]);

		$this->node_count--;

		print_r("Succesed to remove a node " . $node . "\n");

		return $this;
	}

	/* 返回当前给所有的节点 */
	public function get_all_nodes() {
		return array_keys($this->node_position);
	}

	/* 将所有的虚拟节点的哈希值进行排序 */
	public function position_sort() {
		if (!$this->position_sorted) {
			ksort($this->position_node, SORT_REGULAR);
			$this->position_sorted = True;
		}
	}

	/*  将缓存数据映射到节点上 对统一个缓存资源可能会映射到多个节点上，根据参数指定个数 返回节点集合 */
	public function look_up_nodes($object, $request) {
		if (!$request) {
			throw new Negi_exception("The request argument is invalid");
		}

		if (empty($this->position_node)) {
			return array();
		}

		if ($this->node_count == 1) {
			return array_unique(array_values($this->position_node));
		}

		$object_hash = $this->hash_algorithm->hash($object);

		$this->position_sort();
		$results = array();
		$collect = False;

		foreach ($this->position_node as $key => $value) {
			if (!$collect && $key > $object_hash) {
				$collect = True;
			}

			if ($collect && !in_array($value, $results)) {
				$results[] = $value;
			}

			if (count($results) == $request || count($results) == $this->node_count) {
				return $results;
			}
		}

		foreach ($this->position_node as $key => $value) {
			if (!in_array($value, $results)) {
				$results[] = $value;
			}

			if (count($results) == $request || count($request) == $this->node_count) {
				return $results;
			}
		}

		return $results;
	}

	/* 返回存储当前资源的节点服务器 默认是返回一个 */
	public function look_up($object) {
		$results = $this->look_up_nodes($object, 1);

		if (empty($results)) throw new Negi_exception("There is not cache node to store the object");

		print_r("Succesed to look_up the resource " . $object . " in the array " . implode(",", $results) . " \n");

		return $results[0];
	}

	public function __toString() {
		return sprintf('%s{targets:[%s]}', get_class($this), implode(',', $this->get_all_nodes()));
	}
}


$hash = new Negi_hash();

$hash->add_nodes(array('cache-1', 'cache-2', 'cache-3'));

$hash->look_up("object-A");
$hash->look_up("object-B");


$hash->add_node('cache-4');

$hash->look_up('object-C');

$hash->remove_node("cache-3");

$hash->look_up('object-C');
?>
