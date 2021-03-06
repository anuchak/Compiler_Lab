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
  map <char, vector<char> > transition;
  void constructTable(map <int, set <int> > followpos);
  void finalTransition();
  void traverse(node* root);
};

int s = 65;

void dfaTable::traverse(node* root)
{
  if(root == NULL)
    return;
  if(root->c >= 'a' && root->c <= 'z')
    this->input.insert(root->c);
  this->traverse(root->left);
  this->traverse(root->right);
}

void dfaTable::finalTransition(void)
{
  cout<<"\n\n*************** This is the final TRANSITION TABLE ***************\n\n";
  cout<<"-----------------------\n";
//cout<<"State "<<"|"<<" Input | "<<" Result \n";
  cout<<"State "<<" | ";
  for(auto itr = this->input.begin(); itr != this->input.end(); itr++)
  cout<<setw(3)<<*itr<<"   |";
  cout<<"\n";
  cout<<"-----------------------\n";
  for(auto it = this->transition.begin(); it != this->transition.end(); it++)
  {
    cout<<setw(3)<<it->first<<"    | ";
    for(auto itr = it->second.begin(); itr != it->second.end(); itr++)
      cout<<setw(3)<<*itr<<"   |";
    cout<<"\n";
  }
  cout<<"\n\n";
}

void dfaTable::constructTable(map <int, set <int> > followpos)
{
  for(auto it = this->state.begin(); it != this->state.end(); it++)    // for every state
  {
    char t;
    for(auto it1 = this->input.begin(); it1 != this->input.end(); it1++)  // for every input symbol
    {
          t = *it1;                              // t is the input symbol
          set <int> v;                            // vector to contain the new states
          if(it->second.size() == 0)
          break;
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
  	  this->transition[it->first].push_back(next);
  	}
        cout<<"\n";
  }
  this->finalTransition();
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
      // Node->nullable = 0;
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
        if(tleft->nullable && tright->nullable)
        	Node->nullable = 1;
        else
        	Node->nullable = 0;
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
        if(tleft->nullable || tright->nullable)
        	Node->nullable = 1;
        else
        	Node->nullable = 0;
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
  cout<<"Char '";
  cout<<ch<<"'  ";
  if(root->left == NULL)
    cout<<" --- Left child is NULL ";
  else
    cout<<" --- Left child is -> "<<root->left->c<<" ";
  if(root->right == NULL)
    cout<<" --- Right child is NULL ";
  else
    cout<<" --- Right child is -> "<<root->right->c<<" ";
  cout << " --- Firstpos (";
  for(auto it = root->firstpos.begin(); it!=root->firstpos.end(); it++)
    cout<<*it<<" ";
  cout<<") ";
  cout << " --- Lastpos  (";
  for(auto it = root->lastpos.begin(); it!=root->lastpos.end(); it++)
    cout<<*it<<" ";
  cout<<")  ";
  cout<<" --- nullable -> "<<root->nullable;
  cout<<"\n\n";
  traversal(root->left);
  traversal(root->right);
}

string conv(string exp)
{
  stack <char> st;
  string res;
  for(int i = 0; i < exp.size(); i++)
  {
    if(exp[i] >= 'a' && exp[i] <= 'z')
     res.push_back(exp[i]);
    else if(exp[i] == '(')
     st.push(exp[i]);
    else if(exp[i] == '*')
     res.push_back(exp[i]);
    else if(exp[i] == ')')
    {
      while(!st.empty() && st.top() != '(')
      {
        char c = st.top();
        st.pop();
        res.push_back(c);
      }
      st.pop();
    }
    else
    {
      while(!st.empty() && (st.top() <= exp[i]) && st.top() != '(')
      {
        char c = st.top();
        st.pop();
        res.push_back(c);
      }
      st.push(exp[i]);
    }
  }
  while(!st.empty())
  {
    char c = st.top();
    st.pop();
    res.push_back(c);
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
  cout << "The format of the input is to be   (a.b)|c ... ((a|b)*.b ... )   eg. (a|b)*.b.a.a \n\n";
  cout << "Some examples of sample input => (a|b)*.b.a.a        [input form]\n\n";
  cout << "                                 (a|b*)|(a|b)*    [input form]\n\n";
  cout << "                                 (a|b*.(a|b)*.a)*|(b|(a|b)*)   [input form]\n\n";
  cout << "The operator precedence considered is '*' > '.' > '|'\n\n";
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
  traversal(exp.root);
  cout<<"\n";
  map <int, set <int> > followpos;
  followpostable(followpos,exp.root);
  cout<<"\n============ FOLLOWPOS TABLE GENERATED ============\n\n";
  cout<<"-------------------------------\n";
  cout<<"| Node i      | Followpos     |\n";
  cout<<"-------------------------------\n";
  for(auto it = followpos.begin(); it != followpos.end(); it++)
  {
  cout<<"|"<<it->first<<"  "<<charToNum[it->first]<<"\t      | ";
  for(auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
    cout<<*it1<<" ";
  cout<<"\n";
  }
  cout<<"\n";
  dfaTable dfa;
  dfa.traverse(exp.root);
  // dfa.input.insert('a');
  // dfa.input.insert('b');
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
