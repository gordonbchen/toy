from typing import List


class Solution:
    def foreignDictionary(self, words: List[str]) -> str:
        edges = {}

        alien_dict = [words]
        while len(alien_dict) > 0:
            new_alien_dict = []
            for awords in alien_dict:
                char_map = {}
                chars = []
                for word in awords:
                    if char_map.get(word[0], None) is None:
                        char_map[word[0]] = []
                    chars.append(word[0])
                    char_map[word[0]].append(word[1:])

                for ws in char_map.values():
                    l = 0
                    filtered_ws = []
                    for w in ws:
                        if len(w) < l: return ""
                        l = max(len(w), l)
                        if len(w) > 0: filtered_ws.append(w)

                    if filtered_ws: new_alien_dict.append(filtered_ws)

                for (a, b) in zip(chars, chars[1:]):
                    if not a in edges:
                        edges[a] = set()
                    edges[a].add(b)
                if len(chars) == 1 and chars[0] not in edges:
                    edges[chars[0]] = set()
                if len(chars) > 1 and chars[-1] not in edges:
                    edges[chars[-1]] = set()

            alien_dict = new_alien_dict

            print(new_alien_dict)
            print(edges, '\n')

        to_from = {}
        for a in edges:
            to_from[a] = set()
            for b in edges:
                if a == b: continue
                if a in edges[b]: to_from[a].add(b)
        print(to_from)

        topo = []
        while to_from:
            removed = False
            for k in to_from:
                if len(to_from[k]) == 0:
                    topo.append(k)
                    to_from.pop(k)
                    for v in to_from:
                        if k in to_from[v]:
                            to_from[v].remove(k)
                    removed = True
                    break
            if not removed:
                return ""

        return "".join(topo)


if __name__ == "__main__":
    wordss = [["abc","bcd","cde"], ["wrtkj", "wrt"],
              ["h", "ab", "aa", "ba"], ["hrn", "hrf", "er", "enn", "rfnn"],
              ["baa","abcd","abca","cab","cad"],
    ["abcdefgh","bdefghij","cghij","dfghij","efghij","fghij","ghij","hij","ij","j","abcdefghi","bdefghijk","cghijk","dfghijk","efghijk","fghijk","ghijk","hijk","ijk","jk","k"]]
    # wordss = [['z', 'o']]
    for words in wordss:
        print(words, '\n')
        print(Solution().foreignDictionary(words))
