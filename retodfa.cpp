#include <bits/stdc++.h>
using namespace std;

struct node
{
  char c;
  node* left;
  node* right;
  int nullable;
  set <int> firstpos,lastpos;
};

int num = 1;
map <int, char> charToNum;

class parseTree
{
  public:
  node* root;
  node* constructTree(string s);
};

class dfaTable
{
  public:
  set <char> input;
  map <char, set <int> > state;
  // map <char, pair <char,char> > transition;
  void constructTable(map <int, set <int> > followpos);
  void finalTransition();
};

int s = 65;

void dfaTable::constructTable(map <int, set <int> > followpos)
{
  for(auto it = this->state.begin(); it != this->state.end(); it++)    // for every state
  {
    char t;
    for(auto it1 = this->input.begin(); it1 != this->input.end(); it1++)  // for every input symbol
    {
      t = *it1;                                                        // t is the input symbol
  	  set <int> v;                                                     // vector to contain the new states
  	  for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++)   // for every number in each state
  	  {
        if(charToNum[*it2] == t)                                   // if char number matches to state number
  	    {
  	   	  for(auto it3 = followpos[*it2].begin(); it3 != followpos[*it2].end(); it3++)
  	   	  {
  	   	    v.insert(*it3);
  	   	  }
  	   	}
  	  }
  	  cout<<it->first<<" for input "<<t<<" -> ";
  	  for(auto i = v.begin(); i != v.end(); i++)
  	   	cout<<*i<<" ";
  	  char next = '!';
  	  for(auto i = this->state.begin(); i != this->state.end(); i++)
  	  {
  	   	string s1,s2;
  	    for(auto j = i->second.begin(); j != i->second.end(); j++)
  	   	  s1.push_back((*j)+48);
  	   	for(auto j = v.begin(); j != v.end(); j++)
  	   	  s2.push_back((*j)+48);
  	   	if(s1==s2)
  	   	  next=i->first;
  	  }
  	  if(next == '!')
  	  {
  	   	next = ++s;
  	  	for(auto i = v.begin(); i != v.end(); i++)
  	   	  this->state[next].insert(*i);
  	   	cout<<" State => "<<next<<" (new) ";
  	   	for(auto i = v.begin(); i != v.end(); i++)
  	   	  if(*i == num)
  	   	  {
  	   	  	cout<<"Final state";
  	   	  	break;
  	   	  }
  	   	cout<<"\n";
  	  }
  	  else
  	  {
  	  	cout<<" State => "<<next<<"\n";
  	  }
  	  // this->transition[it->first].first = t;
  	  // this->transition[it->first].second = next;
  	}
  	cout<<"\n";
  }
  // this->finalTransition();
}

stack <node*> st;
void traversal(node* root);

node* parseTree::constructTree(string s)
{
  node* r = NULL;
  for(int i=0; i<s.length(); i++)
  {
    if(s[i]!='*' && s[i]!='|' && s[i]!='.')
    {
      node *Node = new node;
      Node->c = s[i];
      Node->left = Node->right = NULL;
      Node->nullable = 0;
      Node->firstpos.insert(num);
      Node->lastpos.insert(num);
      charToNum[num]=s[i];
      num++;
      st.push(Node);
    }
    else if(s[i]=='*')
    {
      node *Node = new node;
      Node->c = s[i];
      Node->right = NULL;
      node *t = st.top();
      st.pop();
      Node->left = t;
      Node->nullable = 1;
      for(auto it = t->firstpos.begin(); it != t->firstpos.end(); it++)
        Node->firstpos.insert(*it);
      for(auto it = t->lastpos.begin(); it != t->lastpos.end(); it++)
        Node->lastpos.insert(*it);
      if(st.empty())
      {
        r=Node;
      }
      st.push(Node);
      // traversal(r);
      // cout<<"\n";
    }
    else
    {
      node *Node = new node;
      Node->c = s[i];
      node *tright = st.top();
      st.pop();
      node *tleft = st.top();
      st.pop();
      Node->left = tleft;
      Node->right = tright;
      Node->nullable = 0;
      if(s[i] == '.')
      {
        if(tleft->nullable)
        {
          for(auto it = tleft->firstpos.begin(); it != tleft->firstpos.end(); it++)
            Node->firstpos.insert(*it);
          for(auto it = tright->firstpos.begin(); it != tright->firstpos.end(); it++)
            Node->firstpos.insert(*it);
        }
        else
        {
          for(auto it = tleft->firstpos.begin(); it != tleft->firstpos.end(); it++)
            Node->firstpos.insert(*it);
        }
        if(tright->nullable)
        {
          for(auto it = tleft->lastpos.begin(); it != tleft->lastpos.end(); it++)
            Node->lastpos.insert(*it);
          for(auto it = tright->lastpos.begin(); it != tright->lastpos.end(); it++)
            Node->lastpos.insert(*it);
        }
        else
        {
          for(auto it = tright->lastpos.begin(); it != tright->lastpos.end(); it++)
            Node->lastpos.insert(*it);
        }
      }
      if(s[i] == '|')
      {
        for(auto it = tleft->firstpos.begin(); it != tleft->firstpos.end(); it++)
            Node->firstpos.insert(*it);
        for(auto it = tright->firstpos.begin(); it != tright->firstpos.end(); it++)
            Node->firstpos.insert(*it);
        for(auto it = tleft->lastpos.begin(); it != tleft->lastpos.end(); it++)
            Node->lastpos.insert(*it);
        for(auto it = tright->lastpos.begin(); it != tright->lastpos.end(); it++)
            Node->lastpos.insert(*it);
      }
      if(st.empty())
      {
         r=Node;
      }
      st.push(Node);
      // traversal(r);
      // cout<<"\n";
    }
  }
  return r;
}

void traversal(node* root)
{
  if(root == NULL)
    return;
  char ch = root->c;
  cout<<ch<<" ";
  if(root->left == NULL)
    cout<<"L NULL ";
  else
    cout<<"L -> "<<root->left->c<<" ";
  if(root->right == NULL)
    cout<<"R NULL ";
  else
    cout<<"R -> "<<root->right->c<<" ";
  cout << "Firstpos (";
  for(auto it = root->firstpos.begin(); it!=root->firstpos.end(); it++)
    cout<<*it<<",";
  cout<<") ";
  cout << "Lastpos (";
  for(auto it = root->lastpos.begin(); it!=root->lastpos.end(); it++)
    cout<<*it<<",";
  cout<<")  ";
  cout<<"nullable -> "<<root->nullable;
  cout<<"\n";
  traversal(root->left);
  traversal(root->right);
}

string conv(string regex)
{
  string res;
  stack <char> s1;
  int brac_marker = 0;
  for(int i=0; i<regex.length(); i++)
  {
    if((regex[i]>='a' && regex[i]<='z') || (regex[i]>='A' && regex[i]<='Z'))
      res.push_back(regex[i]);
    else if(regex[i]=='(')
    {
      s1.push(regex[i]);
      brac_marker++;
    }
    else if(regex[i]==')')
    {
      char c;
      while(s1.top() != '(')
      {
        c = s1.top();
        s1.pop();
        res.push_back(c);
      }
      s1.pop();
      brac_marker--;
    }
    else if((regex[i] == '|' || regex[i] == '.') && !brac_marker)
      res.push_back(regex[i]);
    else if(regex[i] == '*')
      res.push_back(regex[i]);
    else
     s1.push(regex[i]);
  }
  return res;
}

void followpostable(map <int, set <int> > &followpos, node* root)
{
  if(!root)
   return;
  if(root->c == '*')
  {
    for(auto it = root->lastpos.begin(); it != root->lastpos.end(); it++)
      for(auto it1 = root->firstpos.begin(); it1 != root->firstpos.end(); it1++)
      {
        followpos[*it].insert(*it1);
        // cout << *it << " " << *it1 << "\n";
      }
  }
  if(root->c == '.')
  {
    for(auto it = root->left->lastpos.begin(); it != root->left->lastpos.end(); it++)
      for(auto it1 = root->right->firstpos.begin(); it1 != root->right->firstpos.end(); it1++)
      {
        followpos[*it].insert(*it1);
        // cout << *it << " " << *it1 << "\n";
      }
  }
  followpostable(followpos,root->left);
  followpostable(followpos,root->right);
}

int main()
{
  system("clear");
  cout << "\n\n************************************* REGULAR EXPRESSION TO DFA *************************************\n\n";
  cout << "The format of the input is to be (( ... (A|B)*.B) ... )*|B). ... ))  eg. ((((a|b*).b).a)|b*)\n\n";
  cout << "Some examples of sample input => (a|b)*baa        => ((((a|b)*.b).a).a) [input form]\n\n";
  cout << "                                 (a|b*)|(a|b)*    => ((a|b*)|(a|b)*)    [input form]\n\n";
  cout << "                                 (a|b*(a|b)*a)*|(b|(a|b)*) => ((a|((b*.(a|b)*).a))*|(b|(a|b)*)) [input form]\n\n";
  cout << "Enter regular expression -> ";
  string regex;
  cin >> regex;
  cout<<"\n";
  string postfix;
  postfix=conv(regex);
  cout<<"The regular expression in postfix form is -> ";
  cout<<postfix<<"\n\n";
  parseTree exp;
  exp.root = exp.constructTree(postfix);
  node *Node = new node;
  Node->c = '.';
  Node->nullable = 0;
  Node->right = new node;
  Node->right->c = '#';
  Node->right->right = NULL;
  Node->right->left = NULL;
  Node->right->nullable = 0;
  Node->right->firstpos.insert(num);
  Node->right->lastpos.insert(num);
  Node->left = exp.root;
  exp.root = Node;
  node* tleft = exp.root->left;
  node* tright = exp.root->right;
  if(tleft->nullable)
  {
    for(auto it = tleft->firstpos.begin(); it != tleft->firstpos.end(); it++)
      Node->firstpos.insert(*it);
    for(auto it = tright->firstpos.begin(); it != tright->firstpos.end(); it++)
      Node->firstpos.insert(*it);
  }
  else
  {
    for(auto it = tleft->firstpos.begin(); it != tleft->firstpos.end(); it++)
      Node->firstpos.insert(*it);
  }
  exp.root->lastpos.insert(num);
  // traversal(exp.root);
  cout<<"\n";
  map <int, set <int> > followpos;
  followpostable(followpos,exp.root);
  cout<<"\n============ FOLLOWPOS TABLE GENERATED ============\n\n";
  cout<<"###############################\n";
  cout<<"| Node i      | Followpos      \n";
  cout<<"###############################\n";
  for(auto it = followpos.begin(); it != followpos.end(); it++)
  {
  cout<<"|"<<it->first<<"  "<<charToNum[it->first]<<"\t      | ";
  for(auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
    cout<<*it1<<" ";
  cout<<"\n";
  }
  cout<<"\n";
  dfaTable dfa;
  dfa.input.insert('a');
  dfa.input.insert('b');
  cout<<"Stating state is A -> ( ";
  for(auto it = exp.root->firstpos.begin(); it != exp.root->firstpos.end(); it++)
  {
    dfa.state['A'].insert(*it);
    cout<<*it<<" ";
  }
  cout<<")\n\n";
  dfa.constructTable(followpos);
  return 0;
}
