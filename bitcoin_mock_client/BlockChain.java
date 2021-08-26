// Block Chain should maintain only limited block nodes to satisfy the functions
// You should not have all the blocks added to the block chain in memory 
// as it would cause a memory overflow.
import java.util.ArrayList;
import java.util.HashMap;


public class BlockChain {
    public static final int CUT_OFF_AGE = 10;

    class BlockNode{
        Block content;
        ArrayList<BlockNode> children;
        int height;
        UTXOPool utxopool;
        public BlockNode(){
        }
        public BlockNode(Block block, BlockNode parent){
            height = parent.height+1;
            content = block;
            children = new ArrayList<BlockNode>();
            parent.children.add(this);
            
            //making this's utxopool from it's parents
            utxopool = new UTXOPool(parent.utxopool);
            for(Transaction tx : block.getTransactions()){
                for(Transaction.Input input : tx.getInputs()){
                    UTXO temp = new UTXO(input.prevTxHash, input.outputIndex);
                    if(utxopool.contains(temp)){
                        utxopool.removeUTXO(temp);
                    }
                }
                for(int i = 0 ; i < tx.getOutputs().size() ; i++){
                    UTXO temp = new UTXO(tx.getHash(), i);
                    utxopool.addUTXO(temp, tx.getOutput(i));
                }
            }
        }
        public UTXOPool getUTXOPoolCopy() {
            return new UTXOPool(utxopool);
        }
    }

    /**
     * create an empty block chain with just a genesis block. Assume {@code genesisBlock} is a valid
     * block
     */


    public TransactionPool txPool;
    //the UTXOPool for each corresponding BlockNode. key is the hash corresponding to the block, value is UTXOPool
    private HashMap<byte[], BlockNode> allNodes;
    private BlockNode maxHeightBN;
    private BlockNode firstBN;

    public BlockChain(Block genesisBlock) {
        BlockNode firstBN = new BlockNode();
        firstBN.content = genesisBlock;
        firstBN.children = new ArrayList<BlockNode>();
        firstBN.height = 1;
        UTXOPool temp = new UTXOPool();
        for(Transaction tx : genesisBlock.getTransactions()){
            for(int i = 0 ; i < tx.getOutputs().size() ; i++){
                UTXO utxoTemp = new UTXO(tx.getHash(), i);
                temp.addUTXO(utxoTemp, tx.getOutput(i));
            }
        }
        firstBN.utxopool = temp;
        maxHeightBN = firstBN;
        
        txPool = new TransactionPool();

        allNodes = new HashMap<byte[], BlockNode>();
    }
    /**
     * Helper function: returns the index of the children with the maximum block height of a BlockNode
     * @param bn
     * @return
     */
    private int findMaxChildIndex(BlockNode bn){
        if(bn == null ||bn.children.size() == 0){
            return -1;
        } else {
            int max = bn.children.get(0).height;
            int maxIndex = 0;
            for(int i = 0 ; i < bn.children.size() ; i++){
                if(bn.children.get(i).height > max){
                    max = bn.children.get(i).height;
                    maxIndex = i;
                }
            }
            return maxIndex;
        }
    }

    /** Get the maximum height block */
    //if two subchains are the same height, it will choose the first chain
    public Block getMaxHeightBlock() {
        return getMaxHeightBlockNode().content;
    }

    //helper
    public BlockNode getMaxHeightBlockNode(){
        return maxHeightBN;
    }

    /** Get the UTXOPool for mining a new block on top of max height block */
    public UTXOPool getMaxHeightUTXOPool() {
        return maxHeightBN.utxopool;
    }

    /** Get the transaction pool to mine a new block */
    public TransactionPool getTransactionPool() {
        return txPool;
    }

    /**
     * Helper function that finds the BlockNode with a given hash
     * Uses depth first search, starting from root
     */
    private BlockNode findBlockNode(byte[] hash){
        return allNodes.get(hash);
    }

    /**
     * Add {@code block} to the block chain if it is valid. For validity, all transactions should be
     * valid and block should be at {@code height > (maxHeight - CUT_OFF_AGE)}.
     * 
     * <p>
     * For example, you can try creating a new block over the genesis block (block height 2) if the
     * block chain height is {@code <=
     * CUT_OFF_AGE + 1}. As soon as {@code height > CUT_OFF_AGE + 1}, you cannot create a new block
     * at height 2.
     * 
     * @return true if block is successfully added
     */
    public boolean addBlock(Block block) {
        //validity check
        if(block == null){ return false;}
        if(block.getPrevBlockHash() == null){ return false;}
        BlockNode parent = findBlockNode(block.getPrevBlockHash());
        if(parent == null){ return false;}
        TxHandler handler = new TxHandler(parent.getUTXOPoolCopy());
        Transaction[] txs = block.getTransactions().toArray(new Transaction[0]);
        Transaction[] validTxs = handler.handleTxs(txs);
        if(validTxs.length != txs.length){ return false;}
        if(parent.height + 1 <= maxHeightBN.height - CUT_OFF_AGE){
            return false;
        }

        //deleting transactions from txPool
        for(Transaction tx : block.getTransactions()){
            if(!tx.isCoinbase()){
                txPool.removeTransaction(tx.getHash());
            }
        }

        //adding the block
        BlockNode bn = new BlockNode(block, parent);
        allNodes.put(block.getHash(), bn);

        //adjust the blockchain if this is the new longest chain
        if(parent.height + 1 > maxHeightBN.height){
            maxHeightBN = bn;
            byte[] firstBNbytes = firstBN.content.getHash();
            firstBN = firstBN.children.get(findMaxChildIndex(firstBN));
            allNodes.remove(firstBNbytes);
        }

        return true;
    }

    /** Add a transaction to the transaction pool */
    public void addTransaction(Transaction tx) {
        txPool.addTransaction(tx);
    }
}