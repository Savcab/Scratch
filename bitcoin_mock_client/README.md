What I should've done differently after reading other people's versions on Github
    First of all, the BlockNode was a good idea, other people had it too

    Secondly, instead of having a children as BlockNodes, I should just have a big variable that acts as a libraray for all the data. It would be a HashMap with byte as key, and BlockNode as value. So instead of having a ArrayList<BlockNode> for children, I can just save all current BlockNodes in the variable and have ArrayList<byte[]> for children. 

    I had a HashMap<byte[], UTXOPool> for keeping track of all UTXOPools. I should've just made a block's UTXO pool part of it's private data member, and used the hashmap described in the paragraph above.

    I need to think things out more before I start typing.

    Instead of tracing down all the children in order to find the max height node, the other person just had maxheightnode be a paramter of the blockchain, and if the proposed height for a new block is bigger than the maxheightnode, it would just become the maxheightnode